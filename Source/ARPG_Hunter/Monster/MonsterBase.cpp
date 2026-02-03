// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Subsystem/DataManager/DataManager.h"
#include "Data/MonsterData.h"
#include "Component/StatComponent.h"
#include "Controller/MonsterAIController.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"
#include "Subsystem/ObjectPool/ObjectPoolManager.h"
#include "UI/Actor/DamageFont.h"
#include "GameMode/GameState/CombatGameState.h"

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

void AMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
#pragma region IsTest

	if (bIsTest)
	{
		AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
			AnimInstance->OnMontageEnded.AddDynamic(this, &AMonsterBase::OnAnimMontageEnd);

		return;
	}
#pragma endregion
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

#pragma region IsTest
	if (bIsTest) 
	{
		TMap<ECharacterStatType, uint32> BaseStat;
		for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
		{
			ECharacterStatType type = static_cast<ECharacterStatType>(i);
			BaseStat.Add(type, 20);
		}

		StatComp->Init(BaseStat);
		return;
	}
#pragma endregion
}

void AMonsterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StatComp->Clear();
}

void AMonsterBase::Init(const FMonsterInitParam& _param)
{
	ID = _param.ID;
	SectionID = _param.SectionIndex;
	SetActorLocation(_param.Location);
	SetActorRotation(_param.Rotation);

	FMonsterData* Data = GetData();

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
	if (_montage == GetAttackMontage(CurAttackMontageIdx) || _montage == GetHitMontage())
		OnAttackMontageEnded.ExecuteIfBound();

	SetWalkable(true);
}

void AMonsterBase::SetWalkable(bool _bIsWalkable)
{
	GetCharacterMovement()->MaxWalkSpeed = _bIsWalkable ? GetData()->MoveSpeed : 0.0f;
}

FMonsterData* AMonsterBase::GetData() const
{
	return GetGameInstance()->GetSubsystem<UDataManager>()->GetMonsterData(ID);
}

void AMonsterBase::HitBy(const FHitInfo& _hitInfo)
{
	StatComp->TakeDamage(_hitInfo.Damage);
	StatComp->TakeStaminaDamage(_hitInfo.StaggerDamage);

	if (UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>()) 
	{
		ADamageFont* ADamage = Cast<ADamageFont>(ObjectPool->Get(ADamageFont::StaticClass()));
		ADamage->SetActorLocation(WidgetComp->GetComponentLocation());
		ADamage->UpdateUI(_hitInfo.Damage, false);
		ADamage->ShowUI();
	}

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
	SetWalkable(false);
}

void AMonsterBase::Attack()
{
	TObjectPtr<UAnimMontage> AttackMontage = GetAttackMontage(CurAttackMontageIdx);

	if (AttackMontage == nullptr || AnimInstance->Montage_IsPlaying(AttackMontage))
		return;

	AnimInstance->Montage_Play(AttackMontage);
	SetWalkable(false);
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

	for (const FHitResult& hit : HitResults)
	{
		IHitable* Hitable = Cast<IHitable>(hit.GetActor());

		if (Hitable)
		{
			FHitInfo HitInfo
			{
				StatComp->GetStat(ECharacterStatType::ATTACK),
				0,
				this,
				0
			};
			Hitable->HitBy(HitInfo);
		}
	}
}

void AMonsterBase::OnDead()
{
	// 사망 시, 오브젝트 풀로 복귀
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	AICon->StopBT();

	// 충돌 무시 처리
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Corpse")));
	
	// 몬스터 사망 이벤트 호출
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	ACombatGameState* GameState = GameMode->GetGameState<ACombatGameState>();
	GameState->StageEventBus[EStageEvent::HUNT].Broadcast({ SectionID, this });

	// 일반적으로 ACombatGameMode에서 오브젝트 풀링 등록하며, 이벤트에 구독해뒀을 것
	OnMonsterDead.ExecuteIfBound(this);
}

bool AMonsterBase::IsDead()
{
	return StatComp->IsDead();
}

EMonsterType AMonsterBase::GetType()
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

void AMonsterBase::ApplyEffect(TSubclassOf<class UEffect> _effectClass, FEffectParam* _effectParam)
{
	StatComp->ApplyEffect(_effectClass, _effectParam);
}

void AMonsterBase::KnockBack(const FHitInfo& _hitInfo)
{
	FVector Dir = GetActorLocation() - _hitInfo.Attacker->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();
	LaunchCharacter(Dir * _hitInfo.KnockBackStrength, true, true);
}
