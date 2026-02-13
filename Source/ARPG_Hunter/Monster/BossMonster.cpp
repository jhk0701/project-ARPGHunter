// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Define/Enum.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Controller/MonsterAIController.h"
#include "Data/MonsterData.h"
#include "Component/StatComponent.h"
#include "Component/ActionComponent/MonsterActionComponent.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"

ABossMonster::ABossMonster()
{
	ActionComp = CreateDefaultSubobject<UBossActionComponent>(TEXT("BossActionComp"));

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_BossMonster.BT_BossMonster'"));
	if (BTFinder.Succeeded())
		SetBehaviorTree(BTFinder.Object);

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_BossMonster.BB_BossMonster'"));
	if (BBFinder.Succeeded())
		SetBlackboardData(BBFinder.Object);

	static ConstructorHelpers::FClassFinder<UUserWidget> UserWidgetFinder(TEXT("/Game/06-UI/WBP_BossStatusBar.WBP_BossStatusBar_C"));
	if (UserWidgetFinder.Succeeded())
		StatusBarClass = UserWidgetFinder.Class;

	ActionTotalWeights.Init(0.0f, static_cast<uint8>(EMonsterAttackType::END));
}

void ABossMonster::BeginPlay()
{
	Super::BeginPlay();

	// UI 생성
	if (StatusBarClass)
	{
		StatusBar = CreateWidget<UUWMonsterStatusBar>(GetWorld(), StatusBarClass);
		TObjectPtr<UUWBossMonsterStatusBar> StatusBarInst = Cast<UUWBossMonsterStatusBar>(StatusBar);

		UUWBossMonsterStatusBar* BossUI = Cast<UUWBossMonsterStatusBar>(StatusBar);
		GetStatComp()->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(BossUI, &UUWBossMonsterStatusBar::SetHealthBarPercent);
		GetStatComp()->GetResourceEvent(ECharacterResourceType::STAMINA).AddUObject(BossUI, &UUWBossMonsterStatusBar::SetStaggerBarPercent);
		
		UBossActionComponent* BossAction = Cast<UBossActionComponent>(ActionComp);
		BossAction->OnGimicValueChanged.BindUObject(StatusBarInst, &UUWBossMonsterStatusBar::SetGimicStaggerBarPercent);
		BossAction->OnGimicStart.BindLambda(
			[this](EGimicType _type) 
			{
				if(_type == EGimicType::STAGGER)
				{
					TObjectPtr<UUWBossMonsterStatusBar> UI = Cast<UUWBossMonsterStatusBar>(StatusBar);
					UI->ShowGimicStagger();
				}
			}
		);
		BossAction->OnGimicEnd.BindLambda(
			[this](EGimicType _type) 
			{
				if (_type == EGimicType::STAGGER)
				{
					TObjectPtr<UUWBossMonsterStatusBar> UI = Cast<UUWBossMonsterStatusBar>(StatusBar);
					UI->HideGimicStagger();
				}
			}
		);

	}
}

void ABossMonster::Init(const FMonsterInitParam& _param)
{
	Super::Init(_param);

	FMonsterData* MonsterData = GetData();
	for (const FMonsterAction& Action : MonsterData->AttackActions)
	{
		ActionTotalWeights[static_cast<uint8>(Action.Type)] += Action.Weight;
	}

	// UI 초기화
	if (StatusBar)
	{
		UStatComponent* Stat = GetStatComp();

		UUWBossMonsterStatusBar* BossUI = Cast<UUWBossMonsterStatusBar>(StatusBar);
		BossUI->SetHealthBarPercent(Stat->GetResourceValue(ECharacterResourceType::HEALTH), Stat->GetResourceMaxValue(ECharacterResourceType::HEALTH));
		BossUI->SetStaggerBarPercent(Stat->GetResourceValue(ECharacterResourceType::STAMINA), Stat->GetResourceMaxValue(ECharacterResourceType::STAMINA));

		BossUI->ShowUI();
	}
}

float ABossMonster::Attack(EMonsterAttackType _type)
{
	// 가중치에 따른 선별
	FMonsterData* MonsterData = GetData();
	float RandomValue = FMath::FRandRange(0.0f, ActionTotalWeights[static_cast<uint8>(_type)]);
	float Sum = 0.0f;

	for (uint8 i = 0; i < MonsterData->AttackActions.Num(); ++i)
	{
		const FMonsterAction& Action = MonsterData->AttackActions[i];

		if (Action.Type != _type)
			continue;

		Sum += Action.Weight;
		if (RandomValue < Sum)
		{
			ActionComp->SetCurAttackIdx(i);
			break;
		}
	}

	return Super::Attack(_type);
}

void ABossMonster::HitBy(const FHitInfo& _hitInfo)
{
	Super::HitBy(_hitInfo);

	TObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();
	UAnimMontage* HitMontage = GetData()->HitMontage;
	if (HitMontage && IsDead())
	{
		AnimInst->Montage_Play(HitMontage);
		AnimInst->Montage_JumpToSection(FName(TEXT("Dead")), HitMontage);
	}

	// 기믹 처리
	TObjectPtr<UBossActionComponent> BossAction = Cast<UBossActionComponent>(ActionComp);
	if (BossAction->IsInGimic())
		BossAction->InterruptGimic(_hitInfo);
}

void ABossMonster::OnDead()
{
	Super::OnDead();

	if (StatusBar)
	{
		UUWBossMonsterStatusBar* BossUI = Cast<UUWBossMonsterStatusBar>(StatusBar);
		BossUI->HideUI();
	}
}

bool ABossMonster::CanUseGimic()
{
	UStatComponent* Stat = GetStatComp();
	return Stat->GetResourceValue(ECharacterResourceType::SKILL) ==
		Stat->GetResourceMaxValue(ECharacterResourceType::SKILL);
}

void ABossMonster::HandleGimicNotify(EGimicType _type, uint16 _gimicValue)
{
	TObjectPtr<UBossActionComponent> BossAction = Cast<UBossActionComponent>(ActionComp);
	if (_type < EGimicType::END)
	{
		BossAction->StartGimic(_type, _gimicValue);
		
		UStatComponent* Stat = GetStatComp();
		Stat->TryUseResource(ECharacterResourceType::SKILL, Stat->GetResourceMaxValue(ECharacterResourceType::SKILL));
	}
	else
		BossAction->EndGimic();
}