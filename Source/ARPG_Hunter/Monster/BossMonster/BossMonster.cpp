// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster/BossMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Define/Enum.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Controller/MonsterAIController.h"
#include "Data/MonsterData.h"
#include "Data/AttackConfig.h"
#include "Component/StatComponent.h"
#include "SubObject/SubObject.h"
#include "Gimic/GimicAction.h"

ABossMonster::ABossMonster()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_BossMonster.BT_BossMonster'"));
	if (BTFinder.Succeeded())
		SetBehaviorTree(BTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_BossMonster.BB_BossMonster'"));
	if (BBFinder.Succeeded())
		SetBlackboardData(BBFinder.Object);
	
	ActionTotalWeights.Init(0.0f, static_cast<uint8>(EMonsterAttackType::END));
}

void ABossMonster::Init(const FMonsterInitParam& _param)
{
	Super::Init(_param);

	FMonsterData* MonsterData = GetData();
	for (const FName& ActionID : MonsterData->AttackActions)
	{
		FMonsterAction* Action = GetAction(ActionID);
		ActionTotalWeights[static_cast<uint8>(Action->Type)] += Action->Weight;
	}
}

float ABossMonster::Attack(FMonsterAttackParam* _param)
{
	if (nullptr == _param)
		return -1.0f;

	// 가중치에 따른 선별
	FMonsterData* MonsterData = GetData();
	float RandomValue = FMath::FRandRange(0.0f, ActionTotalWeights[static_cast<uint8>(_param->Type)]);
	float Sum = 0.0f;

	for (uint8 i = 0; i < MonsterData->AttackActions.Num(); ++i)
	{
		FMonsterAction* Action = GetAction(MonsterData->AttackActions[i]);

		if (Action->Type != _param->Type)
			continue;

		Sum += Action->Weight;
		if (RandomValue < Sum)
		{
			SetCurAttackIdx(i);
			break;
		}
	}

	return Super::Attack(_param);
}

void ABossMonster::HandleAttackNotify(uint8 _opt)
{
	FMonsterAction* Action = GetCurAction();
	const FAttackDetail& Detail = Action->AttackDetails[_opt];

	if (Detail.Type < EAttackDetailType::RANGED)
		MeleeAttack(Detail);
	else
		RangedAttack(Detail);
}

void ABossMonster::HitBy(const FHitInfo& _hitInfo)
{
	Super::HitBy(_hitInfo);

	UAnimMontage* HitMontage = GetHitMontage();
	if (HitMontage && IsDead())
	{
		TObjectPtr<UAnimInstance> AnimInst = GetAnimInst();
		AnimInst->Montage_Play(HitMontage);
		AnimInst->Montage_JumpToSection(FName(TEXT("Dead")), HitMontage);
	}

	// 기믹 처리
	if (CurGimic)
		CurGimic->Interrupt(_hitInfo);
}

void ABossMonster::MeleeAttack(const FAttackDetail& _detail)
{
	FAttackParam Param;
	Param.Subject = this;
	Param.Channel = ECC_GameTraceChannel3;

	Param.Size = _detail.Size;
	Param.Range = _detail.Range;
	Param.DetailType = _detail.Type;
	Param.OnHitAction =
		[this](FHitResult& _hitResult)
		{
			IHitable* Hitable = Cast<IHitable>(_hitResult.GetActor());

			if (Hitable)
			{
				FHitInfo HitInfo;
				HitInfo.Damage = GetStatComp()->GetStat(ECharacterStatType::ATTACK);
				HitInfo.Attacker = this;
				HitInfo.HitResult = &_hitResult;

				Hitable->HitBy(HitInfo);
			}
		};

	UAttackConfig::Act(Param);
}

void ABossMonster::RangedAttack(const FAttackDetail& _detail)
{
	// 공격 목표 찾기
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	if (nullptr == AICon)
		return;

	UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
	UObject* Target = BBComp->GetValueAsObject(FName(TEXT("Target")));
	if (nullptr == Target)
		return;

	TObjectPtr<AActor> TargetActor = Cast<AActor>(Target);

	UClass* ProjectileClass = _detail.SubObjectClass;
	if (nullptr == ProjectileClass)
		return;

	// 투사체 발사
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	TObjectPtr<ASubObject> Projectile = Cast<ASubObject>(ObjectPool->Get(ProjectileClass));
	Projectile->Init(); // TODO : 투사체 데이터 삽입
	Projectile->SetActorLocation(GetWeaponComp()->GetSocketLocation(FName(TEXT("socket_firePoint"))));
	Projectile->Fire(this, TargetActor);
}

void ABossMonster::OnDead()
{
	Super::OnDead();

	if (CurGimic)
		CurGimic = nullptr;
}

void ABossMonster::StartGimic(EGimicType _type)
{
	// 기믹 시작
	UStatComponent* Stat = GetStatComp();
	Stat->TryUseResource(ECharacterResourceType::SKILL, Stat->GetResourceMaxValue(ECharacterResourceType::SKILL));

	CurGimic = UGimicActionFactory::CreateGimic(GetWorld(), _type);
	CurGimic->Start(this, GetCurAction()->GimicParam);
}

void ABossMonster::ProceedGimic(float _deltaSecond)
{
	if (CurGimic)
		CurGimic->Proceed(_deltaSecond);
}

void ABossMonster::CompleteGimic()
{ 
	// 기믹이 성공적으로 발동
	GetAnimInst()->Montage_JumpToSection(FName(TEXT("Complete")), GetCurrentMontage());
	CurGimic = nullptr;
}

void ABossMonster::StopGimic(EGimicType _type)
{
	// 플레이어가 저지한 경우
	GetAnimInst()->Montage_JumpToSection(EnumToName(_type), GetCurrentMontage());
	CurGimic = nullptr;
}

bool ABossMonster::CanUseSkill()
{
	UStatComponent* Stat = GetStatComp();
	return Stat->GetResourceValue(ECharacterResourceType::SKILL) ==
		Stat->GetResourceMaxValue(ECharacterResourceType::SKILL);;
}
