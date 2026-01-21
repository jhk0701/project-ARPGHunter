// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/MonsterBase.h"
#include "Component/StatComponent.h"
#include "AI/MonsterAIController.h"

AMonsterBase::AMonsterBase()
{ 	
	PrimaryActorTick.bCanEverTick = false;

	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	WeaponComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponComp"));
	WeaponComp->SetupAttachment(GetMesh(), FName(TEXT("socket_weapon")));

	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	StatComp->Init();
	StatComp->OnTakeDamage.AddUObject(this, &AMonsterBase::OnTakeDamage);
}

void AMonsterBase::OnTakeDamage(uint16 _remainHp, uint16 _maxHp)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (HitMontage == nullptr) //  || AnimInstance->Montage_IsPlaying(HitMontage)
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
}

void AMonsterBase::HandleAttackNotify()
{
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

