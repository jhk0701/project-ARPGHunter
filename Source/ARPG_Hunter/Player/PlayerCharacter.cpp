// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

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
	ActionComp->Init(GetMesh()->GetAnimInstance());

	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SmoothRotateToInputDir(DeltaTime);
}

void APlayerCharacter::SmoothRotateToInputDir(float DeltaTime)
{
	// 카메라 정면을 기준으로 입력 방향으로 부드럽게 회전시키기
	if (InputDirection.SizeSquared() <= 0)
		return;

	FRotator TargetRot = GetActorRotation();
	TargetRot.Yaw = GetControlRotation().Yaw + FMath::RadiansToDegrees(FMath::Atan2(InputDirection.Y, InputDirection.X));

	SetActorRotation(FQuat::Slerp(GetActorQuat(), TargetRot.Quaternion(), RotateSpeedToInputDir * DeltaTime));
}

void APlayerCharacter::SetIsSprint(bool _isSprint)
{
	IsSprint = _isSprint;

	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = IsSprint ? SprintSpeed : WalkSpeed;
}


void APlayerCharacter::InputDodge()
{
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	// ActionComp에 회피 액션 사용을 위한 조건 전달
	ActionComp->Dodge(
		InputDirection.SizeSquared() > 0,
		[this](float _staminaUsage) 
		{ return StatComp->TryUseStamina(_staminaUsage); }
	);
}

void APlayerCharacter::InputAttack(EAttackType _type)
{
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	ActionComp->Attack(_type,
		[this](float _staminaUsage) 
		{ return StatComp->TryUseStamina(_staminaUsage); }
	);
}

void APlayerCharacter::EnableNextAction(bool _enable)
{
	if (ActionComp->IsValid() == false)
		return;
	ActionComp->SetEnableNextAction(_enable);
}

void APlayerCharacter::HitBy(uint16 _damage)
{
	if (StatComp->IsDead())
		return;

	StatComp->TakeDamage(_damage);
}

void APlayerCharacter::HandleAttackNotify()
{
	// TODO : 플레이어 캐릭터는 장비에 따라서 공격 호출 트레이스가 달라질 것
	
	// 테스트용 임시 트레이스
	TArray<FHitResult> HitResults;
	bool IsHit = UKismetSystemLibrary::BoxTraceMulti(
		GetWorld(),
		GetActorLocation() + GetActorForwardVector() * 30.0f,
		GetActorLocation() + GetActorForwardVector() * 150.0f,
		FVector(100, 30, 50),
		FRotator::ZeroRotator,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4),
		false,
		{},
		EDrawDebugTrace::ForDuration,
		HitResults,
		true,
		FColor::Red,
		FColor::Green
	);

	if (IsHit)
		GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Red, TEXT("Something Hit"));
}
