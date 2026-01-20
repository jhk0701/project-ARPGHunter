// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/StatComponent.h"
#include "Component/EquipmentComponent.h"
#include "Component/ActionComponent.h"
#include "Data/WeaponTypeData.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Create Comp
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	EquipComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipComp"));
	ActionComp = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComp"));

	TopMeshComp = GetMesh();
	HeadMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMeshComp->SetupAttachment(TopMeshComp);
	BottomMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomMesh"));
	BottomMeshComp->SetupAttachment(TopMeshComp);

	WeaponMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMeshComp->SetupAttachment(TopMeshComp, FName(TEXT("socket_hand_r")));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
#pragma endregion

#pragma region Init Comp
	HeadMeshComp->SetLeaderPoseComponent(TopMeshComp);
	BottomMeshComp->SetLeaderPoseComponent(TopMeshComp);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmComp->bUsePawnControlRotation = true;
#pragma endregion
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// TODO : 플레이어 데이터 받아오기
	StatComp->Init();
	EquipComp->Init();
	ActionComp->Init();

	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SmoothRotateToInputDir();
}

void APlayerCharacter::Dodge()
{
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;
	
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	const FAction& DodgeAction = ActionComp->GetDodgeAction();

	UAnimMontage* DodgeMontage = DodgeAction.Montage;
	if (nullptr == DodgeMontage || AnimInst->Montage_IsPlaying(DodgeMontage))
		return;

	if (StatComp->TryUseStamina(DodgeAction.StaminaUsage) == false)
		return;

	AnimInst->Montage_Play(DodgeMontage);

	if (InputDirection.SizeSquared() > 0)
		AnimInst->Montage_JumpToSection(FName(TEXT("Fwd")), DodgeMontage);
	else
		AnimInst->Montage_JumpToSection(FName(TEXT("Bwd")), DodgeMontage);
}

void APlayerCharacter::SetIsSprint(bool _isSprint)
{
	IsSprint = _isSprint;

	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = IsSprint ? SprintSpeed : WalkSpeed;
}

void APlayerCharacter::Attack()
{
	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Attack Input"));
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
}

void APlayerCharacter::SmoothRotateToInputDir()
{
	if (InputDirection.SizeSquared() <= 0)
		return;

	FRotator TargetRot = GetActorRotation();
	TargetRot.Yaw = GetControlRotation().Yaw + FMath::RadiansToDegrees(FMath::Atan2(InputDirection.Y, InputDirection.X));

	SetActorRotation(FQuat::Slerp(GetActorQuat(), TargetRot.Quaternion(), 0.1f));
}
