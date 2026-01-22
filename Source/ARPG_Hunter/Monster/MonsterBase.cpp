// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Component/StatComponent.h"
#include "AI/MonsterAIController.h"
#include "Kismet/KismetSystemLibrary.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	WeaponComp->SetupAttachment(GetMesh(), FName(TEXT("socket_weapon")));

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
	StatComp->OnTakeDamage.AddUObject(this, &AMonsterBase::OnTakeDamage);
}

void AMonsterBase::OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted)
{
	if (_montage == AttackMontage || _montage == HitMontage)
		OnAttackMontageEnded.ExecuteIfBound();
}

void AMonsterBase::OnTakeDamage(uint16 _remainHp, uint16 _maxHp)
{
	if (HitMontage == nullptr)
		return;
	
	AnimInstance->Montage_Play(HitMontage);
	
	if(StatComp->IsDead())
		AnimInstance->Montage_JumpToSection(FName(TEXT("Dead")), HitMontage);
	else
		AnimInstance->Montage_JumpToSection(FName(TEXT("Hit")), HitMontage);
}

void AMonsterBase::HitBy(uint16 _damage)
{
	StatComp->TakeDamage(_damage);
}

void AMonsterBase::Attack()
{
	AnimInstance->Montage_Play(AttackMontage);
}

void AMonsterBase::HandleAttackNotify()
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
			Hitable->HitBy(StatComp->GetAttack());
	}
}

bool AMonsterBase::IsDead()
{
	return StatComp->IsDead();
}

//// Called every frame
//void AMonsterBase::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

