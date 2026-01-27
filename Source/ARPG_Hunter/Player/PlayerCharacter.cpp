// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Component/StatComponent.h"
#include "Component/EquipmentComponent.h"
#include "Component/ActionComponent.h"
#include "Data/WeaponTypeData.h"
#include "Controller/PlayerCharacterController.h"
#include "UI/PlayerHUD.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWPlayerStatusBar.h"


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

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		APlayerHUD* PlayerHUD = PlayerController->GetHUD<APlayerHUD>();
		ensure(PlayerHUD);

		UUWPlayerHUD* PlayerUI = Cast<UUWPlayerHUD>(PlayerHUD->GetPlayerUI());
		UUWPlayerStatusBar* StatusBar = PlayerUI->GetPlayerStatusBar();
		
		StatusBar->SetHealthBarPercent(StatComp->GetHealth(), StatComp->GetMaxHealth());
		StatusBar->SetStaminaBarPercent(StatComp->GetStamina(), StatComp->GetMaxStamina());
		StatusBar->SetSkillBarPercent(StatComp->GetSkill(), StatComp->GetMaxSkill());

		// UI 이벤트 바인딩
		StatComp->OnHealthChanged.AddUObject(StatusBar, &UUWPlayerStatusBar::SetHealthBarPercent);
		StatComp->OnStaminaChanged.AddUObject(StatusBar, &UUWPlayerStatusBar::SetStaminaBarPercent);
		StatComp->OnSkillChanged.AddUObject(StatusBar, &UUWPlayerStatusBar::SetSkillBarPercent);
	}
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

void APlayerCharacter::Dodge()
{
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	// ActionComp에 회피 액션 사용을 위한 조건 전달
	ActionComp->PlayDodgeAction(InputDirection.SizeSquared() > 0,
		[this](float _staminaUsage) { return StatComp->TryUseStamina(_staminaUsage); }
	);
}

void APlayerCharacter::Attack(EAttackType _eType)
{
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	ActionComp->PlayAttackAction(_eType,
		[this](float _staminaUsage) { return StatComp->TryUseStamina(_staminaUsage); }
	);
}

void APlayerCharacter::AttackEnd()
{
	// 현재 모든 입력 중 작업 완료 처리
	// 필요한 경우에 각 공격 입력 액션을 분리해서 처리
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	ActionComp->ProcessAttackEnd();
}

void APlayerCharacter::SetActionProcess(EActionProcess _eProcess)
{
	if (ActionComp->IsValid() == false)
		return;
	ActionComp->SetActionProcess(_eProcess);
}

void APlayerCharacter::HitBy(const FHitInfo& _hitInfo)
{
	if (StatComp->IsDead())
		return;

	StatComp->TakeDamage(_hitInfo.Damage);
	ActionComp->PlayHitAction(StatComp->IsDead());

	if (StatComp->IsDead())
		OnDead();
}

bool APlayerCharacter::IsDead()
{
	return StatComp->IsDead();
}

void APlayerCharacter::OnDead()
{
	// TODO : 플레이어 사망 후 처리
	// 던전 실패 UI 표시 등등
}

void APlayerCharacter::HandleAttackNotify(uint8 _opt)
{
	TArray<FHitResult> HitResults;
	bool IsHit = ActionComp->TraceAttack(_opt, HitResults);
	
	if (IsHit)
	{
		uint16 Damage = CalculateBaseDamage();
		
		for (const FHitResult& hit : HitResults)
		{
			IHitable* Hitable = Cast<IHitable>(hit.GetActor());
			if (Hitable == nullptr)
				continue;

			FHitInfo Hit
			{
				CalculateCritical(Damage),
				ActionComp->GetAttackActionStaggerDamage()
			};

			Hitable->HitBy(Hit);
		}
	}
}

uint16 APlayerCharacter::CalculateBaseDamage()
{
	return StatComp->GetAttack() * (1.0f + ActionComp->GetAttackActionDamagePer() * 0.01f);
}

uint16 APlayerCharacter::CalculateCritical(uint16 _damage)
{
	int32 critial = FMath::Rand() % 100;

	if (critial <= StatComp->GetCriticalPer())
		_damage *= (1.0f + StatComp->GetCriticalDamagePer() * 0.01f);

	return _damage;
}

void APlayerCharacter::ApplyEffect(TSubclassOf<UEffect> _effectClass, FEffectParam* _effectParam)
{
	StatComp->ApplyEffect(_effectClass, _effectParam);
}
