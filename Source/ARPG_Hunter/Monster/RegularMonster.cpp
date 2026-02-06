// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/RegularMonster.h"
#include "Components/WidgetComponent.h"

#include "Core/GameMode/CombatGameMode.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Component/StatComponent.h"
#include "Data/MonsterData.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"
#include "UI/Actor/DamageFont.h"

ARegularMonster::ARegularMonster()
{
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
	{
		GetStatComp()->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetHealthBarPercent);
		GetStatComp()->GetResourceEvent(ECharacterResourceType::STAMINA).AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetStaggerBarPercent);
	}
}

void ARegularMonster::OnDead()
{
	Super::OnDead();

	// 몬스터 사망 이벤트 호출
	ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->StageEvent[EStageEvent::HUNT].Broadcast({ GetSectionID(), this});
}

void ARegularMonster::Init(const FMonsterInitParam& _param)
{
	Super::Init(_param);

	// UI 설정
	if (UUWMonsterStatusBar* MonsterStatusBar = Cast<UUWMonsterStatusBar>(WidgetComp->GetWidget()))
	{
		// UI 초기화
		MonsterStatusBar->SetHealthBarPercent(GetStatComp()->GetResourceValue(ECharacterResourceType::HEALTH), GetStatComp()->GetResourceMaxValue(ECharacterResourceType::HEALTH));
		MonsterStatusBar->SetStaggerBarPercent(GetStatComp()->GetResourceValue(ECharacterResourceType::STAMINA), GetStatComp()->GetResourceMaxValue(ECharacterResourceType::STAMINA));
	}
}

void ARegularMonster::HitBy(const FHitInfo& _hitInfo)
{
	Super::HitBy(_hitInfo);

	// 데미지 폰트 UI 출력
	if (UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>())
	{
		ADamageFont* ADamage = Cast<ADamageFont>(ObjectPool->Get(ADamageFont::StaticClass()));
		ADamage->SetActorLocation(WidgetComp->GetComponentLocation() + FVector(0, 0, FMath::RandRange(-20.0f, 50.f)));
		ADamage->UpdateUI(_hitInfo.Damage, _hitInfo.bIsCriticalHit);
		ADamage->ShowUI();
	}
}

void ARegularMonster::KnockBack(const FHitInfo& _hitInfo)
{
	FVector Dir = GetActorLocation() - _hitInfo.Attacker->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();

	LaunchCharacter(Dir * _hitInfo.KnockBackStrength, true, true);
}
