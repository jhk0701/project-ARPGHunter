// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Component/EquipmentComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Init Comp
	EquipComp = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipComp"));

	HeadMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeadMesh"));
	HeadMeshComp->SetupAttachment(GetMesh());
	BottomMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BottomMesh"));
	BottomMeshComp->SetupAttachment(GetMesh());

	// temp
	HandMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMeshComp->SetupAttachment(GetMesh());
	FeetMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FeetMesh"));
	FeetMeshComp->SetupAttachment(GetMesh());

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	SpringArmComp->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

#pragma endregion
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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