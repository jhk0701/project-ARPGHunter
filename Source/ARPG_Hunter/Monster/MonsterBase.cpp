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
#include "Data/EffectData.h"

#include "Define/Debug.h"
#include "Define/Enum.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;
	// 하위에서 필요한 컴포넌트를 넣을 것
	// ActionComp = CreateDefaultSubobject<UMonsterActionComponent>(TEXT("ActionComp")); 

	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	// WeaponComp->SetupAttachment(GetMesh(), FName(TEXT("socket_weapon")));

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	for (uint8 i = 0; i < static_cast<uint8>(EPlayerActionType::END); ++i)
		bReactToPlayerAction.Add(static_cast<EPlayerActionType>(i), false);

	SetEnemyCollisionChannel(ECC_GameTraceChannel3); // Player Collision Channel
}

void AMonsterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FTimerManager& Timer = GetWorld()->GetTimerManager();
	if(Timer.IsTimerActive(OnDeadTimer))
		Timer.ClearTimer(OnDeadTimer);
	if (Timer.IsTimerActive(InitTimer))
		Timer.ClearTimer(InitTimer);
}

void AMonsterBase::Init(const FMonsterInitParam& _param)
{
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();

	ID = _param.ID;
	SectionID = _param.SectionIndex;
	Data = DataManager->GetMonsterData(ID);
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
	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
	if (false == Stat.IsValid())
		return;

	// 레벨 반영 스탯 계산
	TMap<ECharacterStatType, uint32> BaseStat;
	for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
	{
		ECharacterStatType Type = static_cast<ECharacterStatType>(i);
		float Val = DataManager->GetMonsterLvCurve(_param.Lv, EnumToName(Type));
		BaseStat.Add(Type, Data->BaseStat[Type] + static_cast<uint32>(Val));
	}
	Stat->Init(BaseStat);

	// 충돌 설정
	GetCapsuleComponent()->SetCollisionProfileName(FName(TEXT("Monster")));
	
	// 애니메이션 설정
	MeshComp->SetAnimInstanceClass(Data->Config->AnimBP);
	TObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst)
		AnimInst->OnMontageEnded.AddUniqueDynamic(this, &AMonsterBase::OnMontageEnded);

	TObjectPtr<UMonsterActionComponent> MonsterActionComp = GetActionComp<UMonsterActionComponent>();
	if (nullptr == MonsterActionComp)
		return;

	MonsterActionComp->Init(Data, AnimInst, WeaponComp);

	// AI BlackBoard 설정
	if (TObjectPtr<AMonsterAIController> MonsterAI = Cast<AMonsterAIController>(GetController()))
	{
		TObjectPtr<UBlackboardComponent> BBComp = MonsterAI->GetBlackboardComponent();
		check(BBComp);
		
		BBComp->SetValueAsFloat(FName(TEXT("RecoginitionRange")), Data->RecoginitionRange);
		BBComp->SetValueAsFloat(FName(TEXT("AttackRange")), Data->AttackRange);
		BBComp->SetValueAsFloat(FName(TEXT("MoveRangeOnAttack")), Data->MoveRangeOnAttack);
		BBComp->SetValueAsFloat(FName(TEXT("ActivityRange")), Data->ActivityRange);
		BBComp->SetValueAsVector(FName(TEXT("ReturnPoint")), _param.Location);
		BBComp->SetValueAsEnum(FName(TEXT("AlertState")), static_cast<uint8>(EMonsterAlertState::IDLE));

		// BT 재가동
		MonsterAI->EnableController();

		// AI 연관 변수 초기화
		for (uint8 i = 0; i < static_cast<uint8>(EPlayerActionType::END); ++i)
			bReactToPlayerAction[static_cast<EPlayerActionType>(i)] = false;
	}

	SetMovementMode(EMovementMode::MOVE_Flying);
	SetMoveSpeed(false);
	AddActorWorldOffset(FVector(0,0, Capsule->GetUnscaledCapsuleHalfHeight()));

	GetWorld()->GetTimerManager().SetTimer(
		InitTimer, 
		[this]() 
		{
			SetMovementMode(EMovementMode::MOVE_Walking);
			SetMovable(true);
		}, 
		InitDelay, 
		false 
	);
}

void AMonsterBase::SetMovementMode(EMovementMode _mode)
{
	if (UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		MoveComp->SetMovementMode(_mode);
}

void AMonsterBase::OnMontageEnded(UAnimMontage* _montage, bool _bInterrupted)
{
	if (_montage == GetActionComp<UMonsterActionComponent>()->GetCurrentMontage() ||
		_montage == Data->Config->HitMontage)
		OnAttackMontageEnded.ExecuteIfBound();

	if (_montage == Data->Config->ExtraMontage ||
		_montage == Data->Config->HitMontage)
		OnExtraActMontageEnded.ExecuteIfBound();

	if (_bInterrupted == false)
		SetMovable(true);
}

void AMonsterBase::SetMovable(bool _bIsMovable)
{
	bIsMovable = _bIsMovable;
	GetCharacterMovement()->MaxWalkSpeed = _bIsMovable ? MoveSpeed : 0.0f;
}

void AMonsterBase::SetMoveSpeed(bool _bIsChasing)
{
	MoveSpeed = _bIsChasing ? GetData()->ChaseSpeed : GetData()->MoveSpeed;

	if (bIsMovable)
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

bool AMonsterBase::ExtraAct(const FName& _actName)
{
	bool bPlayExtraAct = GetActionComp<UMonsterActionComponent>()->PlayExtraAction(_actName);

	if (bPlayExtraAct)
		SetMovable(false);

	return bPlayExtraAct;
}

float AMonsterBase::Attack(EMonsterAttackType _type, const FName& _opt /*= NAME_None*/)
{
	if (IsDead())
		return -1.0f;

	TObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst->Montage_IsPlaying(Data->Config->HitMontage))
		return -1.0f;

	TObjectPtr<UMonsterActionComponent> MonsterAcion = GetActionComp<UMonsterActionComponent>();

	// 공격 선택
	MonsterAcion->SelectAttack(_type);

	// 공격 실행
	float Interval = MonsterAcion->PlayAttackAction(_opt);
	if (Interval > 0)
		SetMovable(false);

	return Interval;
}

void AMonsterBase::HandleAttackNotify(uint8 _opt)
{
	if (IsDead())
		return;

	TWeakObjectPtr<AMonsterBase> WeakThis(this);

	ActionComp->ProcessAttack(_opt, GetEnemyCollisionChannel(),
		[WeakThis, _opt](TArray<FHitResult>& _hitResult)
		{
			if (false == WeakThis.IsValid())
				return;

			uint16 Damage = ACombatGameMode::CalculateAttack(
				WeakThis->GetStatComp()->GetStat(ECharacterStatType::ATTACK),
				WeakThis->GetActionComp<UMonsterActionComponent>()->GetAttackActionDamagePer(_opt));

			for (FHitResult& Hit : _hitResult)
			{
				IHitable* Hitable = Cast<IHitable>(Hit.GetActor());

				if (Hitable)
				{
					FHitInfo HitInfo;
					HitInfo.Damage = Damage;
					HitInfo.Attacker = WeakThis;
					HitInfo.HitResult = &Hit;

					Hitable->HitBy(HitInfo);
				}
			}
		},
		GetTarget()
	);
}

uint32 AMonsterBase::HitBy(const FHitInfo& _hitInfo)
{
	uint32 Damage = Super::HitBy(_hitInfo);

	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();

	Stat->TakeStaminaDamage(_hitInfo.StaggerDamage);
	ShowDamageUI(_hitInfo.bIsCriticalHit, Damage);

	// 피격 시, 이펙트 출력
	if (_hitInfo.HitResult && Data->Config->VFXOnHit)
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

	SetMovable(false);

	return Damage;
}

void AMonsterBase::OnDead()
{
	Super::OnDead();

	// 사망 시 처리
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	AICon->DisableController();

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

EMonsterType AMonsterBase::GetType() const
{
	return Data->Config->Type;
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
