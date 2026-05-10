// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/Boss/BossMonster.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Define/Enum.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Data/MonsterData.h"
#include "Data/MonsterConfig.h"
#include "Component/Stat/StatComponent.h"
#include "Component/Action/Monster/MonsterActionComponent.h"
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

	// UI 초기화
	if (StatusBar)
	{
		TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
		if (false == Stat.IsValid())
			return;

		TObjectPtr<UUWBossMonsterStatusBar> BossUI = Cast<UUWBossMonsterStatusBar>(StatusBar);
		BossUI->SetHealthBarPercent(Stat->GetResourceValue(ECharacterResourceType::HEALTH), Stat->GetResourceMaxValue(ECharacterResourceType::HEALTH));
		BossUI->SetStaggerBarPercent(Stat->GetResourceValue(ECharacterResourceType::STAMINA), Stat->GetResourceMaxValue(ECharacterResourceType::STAMINA));
	}
}


uint32 ABossMonster::HitBy(const FHitInfo& _hitInfo)
{
	uint32 Damage = Super::HitBy(_hitInfo);

	TObjectPtr<UBossActionComponent> BossAction = Cast<UBossActionComponent>(ActionComp);
	// 피격 처리
	if (IsDead())
	{
		BossAction->PlayHitAction(EMonsterState::DEAD);
		return Damage;
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
				TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
				if (false == Stat.IsValid())
					return;

				Stat->RecoverResource(ECharacterResourceType::STAMINA, Stat->GetResourceMaxValue(ECharacterResourceType::STAMINA));
				SetState(EMonsterState::NORMAL);
			}, 
			3.0f, false);

		return Damage;
	}

	// 기믹 처리
	if (GetState() == EMonsterState::GIMIC)
		BossAction->InterruptGimic(_hitInfo);

	return Damage;
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
	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
	if (false == Stat.IsValid())
		return false;

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
		
		TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
		if (false == Stat.IsValid())
			return;

		Stat->TryUseResource(ECharacterResourceType::SKILL, Stat->GetResourceMaxValue(ECharacterResourceType::SKILL));

		BossAction->StartGimic(_type, _gimicValue);
		SetState(EMonsterState::GIMIC);
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

void ABossMonster::OnAlertStateChanged(EMonsterAlertState _prevState, EMonsterAlertState _nextState)
{
	Super::OnAlertStateChanged(_prevState, _nextState);

	if (nullptr == StatusBar)
		return;

	TObjectPtr<UUWBossMonsterStatusBar> BossUI = Cast<UUWBossMonsterStatusBar>(StatusBar);
	
	if (_prevState == EMonsterAlertState::ENAGE)
		BossUI->HideUI();

	if(_nextState == EMonsterAlertState::ENAGE)
		BossUI->ShowUI();
}

void ABossMonster::TriggerReactForPlayerAction(uint8 _actionType)
{
	// 보스의 경우 플레이어의 아이템 사용에 반응할 것
	if (static_cast<EPlayerActionType>(_actionType) != EPlayerActionType::USE_ITEM ||
		true == GetReactToPlayerAction(EPlayerActionType::USE_ITEM))
		return;

	int r = FMath::Rand() % 100 + 1;
	bool bIsTrue = r <= GetData()->PlayerActionReactProbability;
	SetReactToPlayerAction(EPlayerActionType::USE_ITEM, bIsTrue);

	if (bIsTrue)
	{
		if (AAIController* AICon = Cast<AAIController>(GetController()))
			AICon->GetBlackboardComponent()->SetValueAsBool(FName(TEXT("bPlayerActionTrigger")), bIsTrue);
	}
}
