// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/RegularMonster/RegularMonster.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"

#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Component/StatComponent.h"
#include "Data/MonsterData.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"
#include "UI/Actor/DamageFont.h"

ARegularMonster::ARegularMonster()
{
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
	{
		GetStatComp()->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetHealthBarPercent);
		GetStatComp()->GetResourceEvent(ECharacterResourceType::STAMINA).AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetStaggerBarPercent);
	}
}

void ARegularMonster::ShowDamageUI(bool _bIsCritical, uint32 _damage)
{
	Super::ShowDamageUI(_bIsCritical, _damage);

	// 데미지 폰트 UI 출력
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();

	ADamageFont* ADamage = Cast<ADamageFont>(ObjectPool->Get(ADamageFont::StaticClass()));
	ADamage->SetActorLocation(WidgetComp->GetComponentLocation() + FVector(0, 0, FMath::RandRange(DamageFontYRange.X, DamageFontYRange.Y)));
	ADamage->UpdateUI(_damage, _bIsCritical);
	ADamage->ShowUI();
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

	// 모션 재생
	TObjectPtr<UAnimInstance> AnimInst = GetAnimInst();
	if (GetHitMontage())
	{
		AnimInst->Montage_Play(GetHitMontage());

		if (IsDead())
		{
			AnimInst->Montage_JumpToSection(FName(TEXT("Dead")), GetHitMontage());
			return;
		}
		
		AnimInst->Montage_JumpToSection(FName(TEXT("Hit")), GetHitMontage());
	}

	KnockBack(_hitInfo);
}

void ARegularMonster::KnockBack(const FHitInfo& _hitInfo)
{
	FVector Dir = GetActorLocation() - _hitInfo.Attacker->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();

	LaunchCharacter(Dir * _hitInfo.KnockBackStrength, true, true);
}
