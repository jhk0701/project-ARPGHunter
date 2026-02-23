// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/BossMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Define/Enum.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Data/MonsterData.h"
#include "Data/MonsterConfig.h"
#include "Component/StatComponent.h"
#include "Component/ActionComponent/MonsterActionComponent.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"
#include "UI/Actor/DamageFont.h"

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

void ABossMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(GroggyRecoverTimer))
		TimerManager.ClearTimer(GroggyRecoverTimer);
}

void ABossMonster::Init(const FMonsterInitParam& _param)
{
	Super::Init(_param);

	FMonsterData* MonsterData = GetData();
	for (const FMonsterAction& Action : MonsterData->Config->AttackActions)
	{
		ActionTotalWeights[static_cast<uint8>(Action.Type)] += Action.Weight;
	}

	// UI 초기화
	if (StatusBar)
	{
		UStatComponent* Stat = GetStatComp();

		TObjectPtr<UUWBossMonsterStatusBar> BossUI = Cast<UUWBossMonsterStatusBar>(StatusBar);
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

	for (uint8 i = 0; i < MonsterData->Config->AttackActions.Num(); ++i)
	{
		const FMonsterAction& Action = MonsterData->Config->AttackActions[i];

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

	TObjectPtr<UBossActionComponent> BossAction = Cast<UBossActionComponent>(ActionComp);
	// 피격 처리
	if (IsDead())
	{
		BossAction->PlayHitAction(EMonsterState::DEAD);
		return;
	}
	else if (GetStatComp()->IsStaggering() && GetState() != EMonsterState::GROGGY)
	{
		if (GetState() == EMonsterState::GIMIC)
			BossAction->EndGimic();  // 강제 종료

		SetState(EMonsterState::GROGGY);
		BossAction->PlayHitAction(EMonsterState::GROGGY);

		GetWorld()->GetTimerManager().SetTimer(GroggyRecoverTimer, 
			[this]() 
			{
				TObjectPtr<UStatComponent> Stat = GetStatComp();
				Stat->RecoverResource(ECharacterResourceType::STAMINA, Stat->GetResourceMaxValue(ECharacterResourceType::STAMINA));
				SetState(EMonsterState::NORMAL);
			}, 
			3.0f, false);

		return;
	}

	// 기믹 처리
	if (GetState() == EMonsterState::GIMIC)
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
		if (GetState() != EMonsterState::NORMAL)
			return;
		
		BossAction->StartGimic(_type, _gimicValue);
		SetState(EMonsterState::GIMIC);
		
		UStatComponent* Stat = GetStatComp();
		Stat->TryUseResource(ECharacterResourceType::SKILL, Stat->GetResourceMaxValue(ECharacterResourceType::SKILL));
	}
	else
	{
		BossAction->EndGimic();
		SetState(EMonsterState::NORMAL);
	}
}

void ABossMonster::ShowDamageUI(bool _bIsCritical, uint32 _damage)
{
	Super::ShowDamageUI(_bIsCritical, _damage);

	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	if (TObjectPtr<AActor> Instance = ObjectPool->Get(ADamageFont::StaticClass()))
	{
		TObjectPtr<ADamageFont> DamageFont = Cast<ADamageFont>(Instance);
		DamageFont->SetActorLocation(GetActorLocation() + FVector(0, 0, FMath::FRandRange(DamageFontYRange.X, DamageFontYRange.Y)));
		DamageFont->UpdateUI(_damage, _bIsCritical);
		DamageFont->ShowUI();
	}
}

void ABossMonster::OnTargetFound()
{
	Super::OnTargetFound();

	if (StatusBar) 
	{
		TObjectPtr<UUWBossMonsterStatusBar> BossUI = Cast<UUWBossMonsterStatusBar>(StatusBar);
		BossUI->PlayOpenAnim();
	}
}
