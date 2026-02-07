// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Core/Subsystem/DataManager.h"
#include "Controller/MonsterAIController.h"
#include "Component/StatComponent.h"
#include "Data/MonsterData.h"

#include "Define/Debug.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	// WeaponComp->SetupAttachment(GetMesh(), FName(TEXT("socket_weapon")));

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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

	// AI BlackBoard 설정
	if (AMonsterAIController* MonsterAI = Cast<AMonsterAIController>(GetController()))
	{
		UBlackboardComponent* BBComp = MonsterAI->GetBlackboardComponent();
		check(BBComp);
		
		BBComp->SetValueAsFloat(FName(TEXT("RecoginitionRange")), Data->RecoginitionRange);
		BBComp->SetValueAsFloat(FName(TEXT("AttackRange")), Data->AttackRange);
		BBComp->SetValueAsFloat(FName(TEXT("MoveRangeOnAttack")), Data->MoveRangeOnAttack);

		// BT 재가동
		MonsterAI->RestartBT();
	}
}

void AMonsterBase::OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted)
{
	if (_montage == GetAttackMontage(CurAttackMontageIdx) ||
		_montage == GetHitMontage())
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
	StatComp->TakeDamage(_hitInfo.Damage);
	StatComp->TakeStaminaDamage(_hitInfo.StaggerDamage);

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
	if (GetHitMontage())
	{
		AnimInstance->Montage_Play(GetHitMontage());

		if (StatComp->IsDead())
		{
			AnimInstance->Montage_JumpToSection(FName(TEXT("Dead")), GetHitMontage());
			OnDead();
			return;
		}

		AnimInstance->Montage_JumpToSection(FName(TEXT("Hit")), GetHitMontage());
	}

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

void AMonsterBase::OnDead()
{
	// 사망 시 처리
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	AICon->StopBT();

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
	return GetData()->AttackDatas[_idx].Montage;
}
void AMonsterBase::ApplyEffect(TObjectPtr<UEffectData> _effectData)
{
	StatComp->ApplyEffect(_effectData);
}