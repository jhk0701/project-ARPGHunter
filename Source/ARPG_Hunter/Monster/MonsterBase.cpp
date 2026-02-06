// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Subsystem/DataManager/DataManager.h"
#include "Subsystem/ObjectPool/ObjectPoolManager.h"
#include "GameMode/CombatGameMode.h"
#include "Controller/MonsterAIController.h"
#include "Data/MonsterData.h"
#include "Component/StatComponent.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"
#include "UI/Actor/DamageFont.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	// WeaponComp->SetupAttachment(GetMesh(), FName(TEXT("socket_weapon")));
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(GetRootComponent());

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UWidgetComponent* Widget = GetWidgetComp();
	static ConstructorHelpers::FClassFinder<UUserWidget> StatusUIFinder(TEXT("/Game/06-UI/WBP_MonsterStatusBar.WBP_MonsterStatusBar_C"));
	if (StatusUIFinder.Succeeded() && Widget)
		Widget->SetWidgetClass(StatusUIFinder.Class);

	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetDrawSize(FVector2D(200,30));
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (UUWMonsterStatusBar* MonsterStatusBar = Cast<UUWMonsterStatusBar>(WidgetComp->GetWidget()))
	{
		StatComp->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetHealthBarPercent);
		StatComp->GetResourceEvent(ECharacterResourceType::STAMINA).AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetStaggerBarPercent);
	}
}

void AMonsterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StatComp->Clear();

	FTimerManager& Timer = GetWorld()->GetTimerManager();
	if(Timer.IsTimerActive(OnDeadTimer))
		Timer.ClearTimer(OnDeadTimer);
}

void AMonsterBase::Init(const FMonsterInitParam& _param)
{
	ID = _param.ID;
	SectionID = _param.SectionIndex;
	SetActorLocation(_param.Location);
	SetActorRotation(_param.Rotation);

	Data = GetGameInstance()->GetSubsystem<UDataManager>()->GetMonsterData(ID);

	// 메쉬 설정
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetSkeletalMesh(Data->BodyMesh);

	if (Data->WeaponMesh)
	{
		WeaponComp->SetSkeletalMesh(Data->WeaponMesh);
		WeaponComp->SetHiddenInGame(false);

		FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);
		WeaponComp->AttachToComponent(MeshComp, AttachRules, FName(TEXT("socket_weapon")));
	}
	else
		WeaponComp->SetHiddenInGame(true);

	// Stat 설정
	// TODO : 레벨 반영 스탯 계산
	TMap<ECharacterStatType, uint32> BaseStat;
	for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
	{
		ECharacterStatType type = static_cast<ECharacterStatType>(i);
		BaseStat.Add(type, Data->BaseStat[type]);
	}

	StatComp->Init(BaseStat);

	// 충돌 설정
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Monster")));
	
	// 애니메이션 설정
	MeshComp->SetAnimInstanceClass(Data->AnimBP);
	if (AnimInstance = GetMesh()->GetAnimInstance())
		AnimInstance->OnMontageEnded.AddUniqueDynamic(this, &AMonsterBase::OnAnimMontageEnd);

	// UI 설정
	if (UUWMonsterStatusBar* MonsterStatusBar = Cast<UUWMonsterStatusBar>(WidgetComp->GetWidget()))
	{
		// UI 초기화
		MonsterStatusBar->SetHealthBarPercent(StatComp->GetResourceValue(ECharacterResourceType::HEALTH), StatComp->GetResourceMaxValue(ECharacterResourceType::HEALTH));
		MonsterStatusBar->SetStaggerBarPercent(StatComp->GetResourceValue(ECharacterResourceType::STAMINA), StatComp->GetResourceMaxValue(ECharacterResourceType::STAMINA));
	}

	// AI BlackBoard 설정
	if (AMonsterAIController* MonsterAI = Cast<AMonsterAIController>(GetController()))
	{
		UBlackboardComponent* BBComp = MonsterAI->GetBlackboardComponent();
		check(BBComp);
		
		BBComp->SetValueAsFloat(FName(TEXT("RecoginitionRange")), Data->RecoginitionRange);
		BBComp->SetValueAsFloat(FName(TEXT("AttackRange")), Data->AttackRange);

		// BT 재가동
		MonsterAI->RestartBT();
	}
}

void AMonsterBase::OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted)
{
	if (_montage == GetAttackMontage(CurAttackMontageIdx) || 
		_montage == GetHitMontage())
	{
		OnAttackMontageEnded.ExecuteIfBound();
		SetMovable(true);
	}
}

void AMonsterBase::SetMovable(bool _bIsMovable)
{
	bIsMovable = _bIsMovable;
	GetCharacterMovement()->MaxWalkSpeed = _bIsMovable ? GetData()->MoveSpeed : 0.0f;
}

void AMonsterBase::HitBy(const FHitInfo& _hitInfo)
{
	// 피격 발생
	StatComp->TakeDamage(_hitInfo.Damage);
	StatComp->TakeStaminaDamage(_hitInfo.StaggerDamage);

	// 데미지 폰트 UI 출력
	if (UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>()) 
	{
		ADamageFont* ADamage = Cast<ADamageFont>(ObjectPool->Get(ADamageFont::StaticClass()));
		ADamage->SetActorLocation(WidgetComp->GetComponentLocation());
		ADamage->UpdateUI(_hitInfo.Damage, _hitInfo.bIsCriticalHit);
		ADamage->ShowUI();
	}

	// 피격 시, 이펙트 출력
	if (Data->VFXOnHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			Data->VFXOnHit,
			_hitInfo.HitResult->ImpactPoint,
			_hitInfo.HitResult->ImpactNormal.Rotation(),
			FVector::OneVector,
			true, true,
			ENCPoolMethod::AutoRelease
		);
	}

	// 모션 재생
	if (GetHitMontage() == nullptr)
		return;

	AnimInstance->Montage_Play(GetHitMontage());

	if (StatComp->IsDead())
	{
		AnimInstance->Montage_JumpToSection(FName(TEXT("Dead")), GetHitMontage());
		OnDead();
		return;
	}

	AnimInstance->Montage_JumpToSection(FName(TEXT("Hit")), GetHitMontage());
	SetMovable(false);
}

void AMonsterBase::Attack()
{
	TObjectPtr<UAnimMontage> AttackMontage = GetAttackMontage(CurAttackMontageIdx);

	if (AttackMontage == nullptr || 
		AnimInstance->Montage_IsPlaying(AttackMontage) || 
		AnimInstance->Montage_IsPlaying(GetHitMontage()))
		return;

	AnimInstance->Montage_Play(AttackMontage);
	SetMovable(false);
}

void AMonsterBase::LookAtTarget(const FVector& _targeLocation)
{
	FVector Dir = _targeLocation - GetActorLocation();
	Dir.Z = 0;
	Dir.Normalize();

	FRotator Rot(0, FMath::RadiansToDegrees(FMath::Atan2(Dir.Y, Dir.X)), 0);
	SetActorRotation(Rot);
}

void AMonsterBase::HandleAttackNotify(uint8 _opt)
{
	// 히트 판정
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * GetData()->AttackRange;

	TArray<FHitResult> HitResults;
	bool IsHit = UKismetSystemLibrary::BoxTraceMulti(
		this,
		Start, End,
		FVector(50.0f, 50.0f, 50.0f), GetActorRotation(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3),
		false,	{ this },
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (IsHit == false)
		return;

	for (FHitResult& Hit : HitResults)
	{
		IHitable* Hitable = Cast<IHitable>(Hit.GetActor());

		if (Hitable)
		{
			FHitInfo HitInfo;
			HitInfo.Damage = StatComp->GetStat(ECharacterStatType::ATTACK);
			HitInfo.Attacker = this;
			HitInfo.HitResult = &Hit;

			Hitable->HitBy(HitInfo);
		}
	}
}

void AMonsterBase::OnDead()
{
	// 사망 시 처리
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	AICon->StopBT();

	// 충돌 무시 처리
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Corpse")));
	
	// 몬스터 사망 이벤트 호출
	ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->StageEvent[EStageEvent::HUNT].Broadcast({ SectionID, this });

	FTimerManager& Timer = GetWorld()->GetTimerManager();
	if (Timer.IsTimerActive(OnDeadTimer))
		Timer.ClearTimer(OnDeadTimer);

	Timer.SetTimer(OnDeadTimer, 
		[this]() 
		{
			// 일반적으로 ACombatGameMode에서 오브젝트 풀링 등록하며, 이벤트에 구독해뒀을 것
			OnMonsterDead.ExecuteIfBound(this);

		}, 
		DeadDelay, false
	);
}

bool AMonsterBase::IsDead()
{
	return StatComp->IsDead();
}

EMonsterType AMonsterBase::GetType() const
{
	return GetData()->Type;
}
TObjectPtr<UAnimMontage> AMonsterBase::GetHitMontage() const
{
	return GetData()->HitMontage;
}
TObjectPtr<UAnimMontage> AMonsterBase::GetAttackMontage(int _idx) const
{
	return GetData()->AttackMontages[_idx];
}
void AMonsterBase::ApplyEffect(TObjectPtr<UEffectData> _effectData)
{
	StatComp->ApplyEffect(_effectData);
}

void AMonsterBase::KnockBack(const FHitInfo& _hitInfo)
{
	FVector Dir = GetActorLocation() - _hitInfo.Attacker->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();
	LaunchCharacter(Dir * _hitInfo.KnockBackStrength, true, true);
}
