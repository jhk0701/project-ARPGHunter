// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Core/Subsystem/DataManager.h"
#include "Core/GameMode/CombatGameMode.h"
#include "Controller/MonsterAIController.h"
#include "Component/StatComponent.h"
#include "Component/ActionComponent/MonsterActionComponent.h"

#include "Data/MonsterData.h"
#include "Data/MonsterConfig.h"
#include "Data/Action.h"
#include "Data/EffectData.h"

#include "Define/Debug.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	// 하위에서 필요한 컴포넌트를 넣을 것
	// ActionComp = CreateDefaultSubobject<UMonsterActionComponent>(TEXT("ActionComp")); 

	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	// WeaponComp->SetupAttachment(GetMesh(), FName(TEXT("socket_weapon")));

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	StatComp->OnDead.AddUObject(this, &AMonsterBase::OnDead);
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
	Data = GetGameInstance()->GetSubsystem<UDataManager>()->GetMonsterData(ID);
	CurState = EMonsterState::NORMAL;

	// 메쉬 설정
	USkeletalMeshComponent* MeshComp = GetMesh();
	MeshComp->SetSkeletalMesh(Data->Config->BodyMesh);
	MeshComp->SetRelativeLocation(FVector(0.0f, 0.0f, -Data->Config->HalfHeight * Data->MeshScale));
	MeshComp->SetRelativeScale3D(FVector(Data->MeshScale));

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleHalfHeight(Data->Config->HalfHeight * Data->MeshScale);
	Capsule->SetCapsuleRadius(Data->Config->Radius * Data->MeshScale);

	SetActorLocation(_param.Location);
	SetActorRotation(_param.Rotation);

	if (Data->Config->WeaponMesh)
	{
		WeaponComp->SetSkeletalMesh(Data->Config->WeaponMesh);
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
	MeshComp->SetAnimInstanceClass(Data->Config->AnimBP);
	TObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
		AnimInst->OnMontageEnded.AddUniqueDynamic(this, &AMonsterBase::OnAnimMontageEnd);
	ActionComp->Init(Data, AnimInst, WeaponComp);

	// AI BlackBoard 설정
	if (TObjectPtr<AMonsterAIController> MonsterAI = Cast<AMonsterAIController>(GetController()))
	{
		TObjectPtr<UBlackboardComponent> BBComp = MonsterAI->GetBlackboardComponent();
		check(BBComp);
		
		BBComp->SetValueAsFloat(FName(TEXT("RecoginitionRange")), Data->RecoginitionRange);
		BBComp->SetValueAsFloat(FName(TEXT("AttackRange")), Data->AttackRange);
		BBComp->SetValueAsFloat(FName(TEXT("MoveRangeOnAttack")), Data->MoveRangeOnAttack);

		// BT 재가동
		MonsterAI->RestartBT();
	}

	SetMovable(true);
}

void AMonsterBase::OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted)
{
	if (_montage == ActionComp->GetCurrentMontage() || _montage == Data->Config->HitMontage)
		OnAttackMontageEnded.ExecuteIfBound();

	if (_bInterrupted == false)
		SetMovable(true);
}

void AMonsterBase::SetMovable(bool _bIsMovable)
{
	bIsMovable = _bIsMovable;
	GetCharacterMovement()->MaxWalkSpeed = _bIsMovable ? GetData()->MoveSpeed : 0.0f;
}

void AMonsterBase::HitBy(const FHitInfo& _hitInfo)
{
	// 피격 발생
	uint32 Damage = ACombatGameMode::CalculateDefense(_hitInfo.Damage, StatComp->GetStat(ECharacterStatType::DEFENSE));

	StatComp->TakeDamage(Damage);
	StatComp->TakeStaminaDamage(_hitInfo.StaggerDamage);
	ShowDamageUI(_hitInfo.bIsCriticalHit, Damage);

	// 피격 시, 이펙트 출력
	if (Data->Config->VFXOnHit)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),
			Data->Config->VFXOnHit,
			_hitInfo.HitResult->ImpactPoint,
			_hitInfo.HitResult->ImpactNormal.Rotation(),
			FVector::OneVector,
			true, true,
			ENCPoolMethod::AutoRelease
		);
	}

	/*if (StatComp->IsDead())
		OnDead();*/

	SetMovable(false);
}

float AMonsterBase::Attack(EMonsterAttackType _type)
{
	if (IsDead())
		return -1.0f;

	TObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst->Montage_IsPlaying(Data->Config->HitMontage))
		return -1.0f;

	// 공격
	float Interval = ActionComp->PlayAttackAction();
	if (Interval > 0)
	{
		SetMovable(false);

		// 공격 시 자기 버프 획득
		for (const TObjectPtr<UEffectData>& Effect : ActionComp->GetCurrentAction()->EffectOnStart)
			StatComp->ApplyEffect(Effect);
	}

	return Interval;
}

void AMonsterBase::HandleAttackNotify(uint8 _opt)
{
	if (IsDead())
		return;

	TWeakObjectPtr<AMonsterBase> WeakThis(this);
	TWeakObjectPtr<UAction> WeakAction(ActionComp->GetCurrentAction());

	ActionComp->ProcessAttack(_opt, ECC_GameTraceChannel3,
		[WeakThis, WeakAction](TArray<FHitResult>& _hitResult)
		{
			if (WeakThis.IsValid() == false || WeakAction.IsValid() == false)
				return;

			uint16 Damage = ACombatGameMode::CalculateAttack(
				WeakThis->GetStatComp()->GetStat(ECharacterStatType::ATTACK),
				WeakAction->AttackDamagePer);

			for (FHitResult& hitResult : _hitResult)
			{
				IHitable* Hitable = Cast<IHitable>(hitResult.GetActor());

				if (Hitable)
				{
					FHitInfo HitInfo;
					HitInfo.Damage = Damage;
					HitInfo.Attacker = WeakThis;
					HitInfo.HitResult = &hitResult;

					Hitable->HitBy(HitInfo);
				}
			}
		}
	);
}

void AMonsterBase::OnDead()
{
	// 사망 시 처리
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	AICon->StopBT();

	// 몬스터 사망 이벤트 호출
	ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->StageEvent[EStageEvent::HUNT].Broadcast({ GetSectionID(), this });

	// 충돌 무시 처리
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Corpse")));
	
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

bool AMonsterBase::IsDead() const
{
	return StatComp->IsDead();
}
EMonsterType AMonsterBase::GetType() const
{
	return Data->Config->Type;
}
void AMonsterBase::ApplyEffect(TObjectPtr<UEffectData> _effectData)
{
	StatComp->ApplyEffect(_effectData);
}

TWeakObjectPtr<AActor> AMonsterBase::GetTarget() const
{
	TObjectPtr<AMonsterAIController> AICon = Cast<AMonsterAIController>(GetController());
	if (AICon == nullptr)
		return nullptr;

	TObjectPtr<UBlackboardComponent> BBComp = AICon->GetBlackboardComponent();
	if (BBComp == nullptr)
		return nullptr;

	TWeakObjectPtr<AActor> Target = Cast<AActor>(BBComp->GetValueAsObject(FName(TEXT("Target"))));
	return Target;
}
