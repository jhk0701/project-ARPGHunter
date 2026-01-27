// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Component/StatComponent.h"
#include "Controller/MonsterAIController.h"
#include "UI/UserWidget/UWMonsterStatusBar.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	WeaponComp->SetupAttachment(GetMesh(), FName(TEXT("socket_weapon")));
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(GetRootComponent());

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AMonsterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMonsterBase::OnAnimMontageEnd);
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	StatComp->Init();

	if (UUWMonsterStatusBar* MonsterStatusBar = Cast<UUWMonsterStatusBar>(WidgetComp->GetWidget())) 
	{
		MonsterStatusBar->SetHealthBarPercent(StatComp->GetHealth(), StatComp->GetMaxHealth());
		MonsterStatusBar->SetStaggerBarPercent(StatComp->GetStamina(), StatComp->GetMaxStamina());

		StatComp->OnHealthChanged.AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetHealthBarPercent);
		StatComp->OnStaminaChanged.AddUObject(MonsterStatusBar, &UUWMonsterStatusBar::SetStaggerBarPercent);
	}
}

void AMonsterBase::OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted)
{
	if (_montage == AttackMontage || _montage == HitMontage)
		OnAttackMontageEnded.ExecuteIfBound();
}

void AMonsterBase::HitBy(const FHitInfo& _hitInfo)
{
	StatComp->TakeDamage(_hitInfo.Damage);
	StatComp->TakeStaminaDamage(_hitInfo.StaggerDamage);

	if (HitMontage == nullptr)
		return;

	AnimInstance->Montage_Play(HitMontage);

	if (StatComp->IsDead())
	{
		AnimInstance->Montage_JumpToSection(FName(TEXT("Dead")), HitMontage);
		OnDead();
		return;
	}

	AnimInstance->Montage_JumpToSection(FName(TEXT("Hit")), HitMontage);
}

void AMonsterBase::Attack()
{
	if (AttackMontage == nullptr || AnimInstance->Montage_IsPlaying(AttackMontage))
		return;

	AnimInstance->Montage_Play(AttackMontage);
}

void AMonsterBase::HandleAttackNotify(uint8 _opt)
{
	// 히트 판정
	FVector Loc = GetActorLocation();

	TArray<FHitResult> HitResults;
	bool IsHit = UKismetSystemLibrary::BoxTraceMulti(
		this,
		Loc, Loc + GetActorForwardVector() * AttackRange,
		FVector(50.0f, 50.0f, 50.0f), GetActorRotation(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3),
		false,	{ this },
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (IsHit == false)
		return;

	for (const FHitResult& hit : HitResults)
	{
		IHitable* Hitable = Cast<IHitable>(hit.GetActor());

		if (Hitable)
		{
			FHitInfo HitInfo
			{
				StatComp->GetAttack(),
				0
			};
			Hitable->HitBy(HitInfo);
		}
	}
}

void AMonsterBase::OnDead()
{
	// 사망 시, 오브젝트 풀로 복귀
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	AICon->StopBT();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// TODO : 몬스터 오브젝트 풀로 복귀
}

bool AMonsterBase::IsDead()
{
	return StatComp->IsDead();
}