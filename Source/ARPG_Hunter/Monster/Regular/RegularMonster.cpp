// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/Regular/RegularMonster.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"

#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Component/Stat/StatComponent.h"
#include "Component/Action/Monster/MonsterActionComponent.h"
#include "Data/MonsterData.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"
#include "UI/Actor/DamageFont.h"

ARegularMonster::ARegularMonster()
{
	ActionComp = CreateDefaultSubobject<UMonsterActionComponent>(TEXT("RegularActionComp"));

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTFinder(TEXT("/Script/AIModule.BehaviorTree'/Game/02-BP/Monster/AI/BT_RegularMonster.BT_RegularMonster'"));
	if (BTFinder.Succeeded())
		SetBehaviorTree(BTFinder.Object);
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBFinder(TEXT("/Script/AIModule.BlackboardData'/Game/02-BP/Monster/AI/BB_RegularMonster.BB_RegularMonster'"));
	if (BBFinder.Succeeded())
		SetBlackboardData(BBFinder.Object);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(GetRootComponent());

	static ConstructorHelpers::FClassFinder<UUserWidget> StatusUIFinder(TEXT("/Game/06-UI/WBP_MonsterStatusBar.WBP_MonsterStatusBar_C"));
	if (StatusUIFinder.Succeeded())
		WidgetComp->SetWidgetClass(StatusUIFinder.Class);

	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetDrawSize(FVector2D(200, 30));
}

void ARegularMonster::BeginPlay()
{
	Super::BeginPlay();

	if (UUWMonsterStatusBar* MonsterStatusBar = Cast<UUWMonsterStatusBar>(WidgetComp->GetWidget()))
		GetStatComp()->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetHealthBarPercent);
}

void ARegularMonster::ShowDamageUI(bool _bIsCritical, uint32 _damage)
{
	Super::ShowDamageUI(_bIsCritical, _damage);

	// 데미지 폰트 UI 출력
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	if (TObjectPtr<AActor> Instance = ObjectPool->Get(ADamageFont::StaticClass())) 
	{
		TObjectPtr<ADamageFont> DamageFont = Cast<ADamageFont>(Instance);
		DamageFont->SetActorLocation(WidgetComp->GetComponentLocation() + FVector(0, 0, FMath::RandRange(DamageFontYRange.X, DamageFontYRange.Y)));
		DamageFont->UpdateUI(_damage, _bIsCritical);
		DamageFont->ShowUI();
	}
}

void ARegularMonster::Init(const FMonsterInitParam& _param)
{
	Super::Init(_param);

	// UI 설정
	if (TObjectPtr<UUWMonsterStatusBar> MonsterStatusBar = Cast<UUWMonsterStatusBar>(WidgetComp->GetWidget()))
	{
		MonsterStatusBar->SetHealthBarPercent(GetStatComp()->GetResourceValue(ECharacterResourceType::HEALTH), GetStatComp()->GetResourceMaxValue(ECharacterResourceType::HEALTH));
		WidgetComp->SetVisibility(false);
	}
}

uint32 ARegularMonster::HitBy(const FHitInfo& _hitInfo)
{
	if (GetReactToPlayerAction(EPlayerActionType::ATTACK))
	{
		SetReactToPlayerAction(EPlayerActionType::ATTACK, false);
		ExtraAct(FName(TEXT("Dodge")));
		return 0;
	}

	uint32 Damage = Super::HitBy(_hitInfo);

	// 모션 재생
	GetActionComp<UMonsterActionComponent>()->PlayHitAction(IsDead() ? EMonsterState::DEAD : EMonsterState::NORMAL);
	KnockBack(_hitInfo);

	return Damage;
}

void ARegularMonster::OnAlertStateChanged(EMonsterAlertState _prevState, EMonsterAlertState _nextState)
{
	Super::OnAlertStateChanged(_prevState, _nextState);

	TObjectPtr<UUWMonsterStatusBar> MonsterStatusBar = Cast<UUWMonsterStatusBar>(WidgetComp->GetWidget());

	if (_prevState == EMonsterAlertState::ENAGE) 
	{
		MonsterStatusBar->PlayOpenAnim(true);
	}

	if (_nextState == EMonsterAlertState::ENAGE)
	{
		WidgetComp->SetVisibility(true);
		MonsterStatusBar->PlayOpenAnim();
	}
}

void ARegularMonster::TriggerReactForPlayerAction(uint8 _actionType)
{
	Super::TriggerReactForPlayerAction(_actionType);

	if (EPlayerActionType::ATTACK != static_cast<EPlayerActionType>(_actionType))
		return;

	int r = FMath::Rand() % 100 + 1;
	SetReactToPlayerAction(EPlayerActionType::ATTACK, r <= GetData()->PlayerActionReactProbability);
}

void ARegularMonster::KnockBack(const FHitInfo& _hitInfo)
{
	FVector Dir = GetActorLocation() - _hitInfo.Attacker->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();

	LaunchCharacter(Dir * _hitInfo.KnockBackStrength * 10.0f, true, false);
}