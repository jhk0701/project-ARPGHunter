// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Component/EquipmentComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Create Comp
	EquipComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipComp"));

	TopMeshComp = GetMesh();
	HeadMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMeshComp->SetupAttachment(TopMeshComp);
	BottomMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomMesh"));
	BottomMeshComp->SetupAttachment(TopMeshComp);

	// temp
	HandMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMeshComp->SetupAttachment(TopMeshComp);
	FeetMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	FeetMeshComp->SetupAttachment(TopMeshComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
#pragma endregion

#pragma region Init Comp
	HeadMeshComp->SetLeaderPoseComponent(TopMeshComp);
	BottomMeshComp->SetLeaderPoseComponent(TopMeshComp);
	HandMeshComp->SetLeaderPoseComponent(TopMeshComp);
	FeetMeshComp->SetLeaderPoseComponent(TopMeshComp);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bInheritPitch = true;
	SpringArmComp->bInheritYaw = true;
	SpringArmComp->bInheritRoll = true;
#pragma endregion
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = WalkSpeed;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::SetIsSprint(bool _isSprint)
{
	IsSprint = _isSprint;

	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = IsSprint ? SprintSpeed : WalkSpeed;
}
