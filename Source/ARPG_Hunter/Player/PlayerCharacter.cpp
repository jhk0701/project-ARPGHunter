// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/WidgetComponent.h"

#include "Component/StatComponent.h"
#include "Component/EquipmentComponent.h"
#include "Component/ActionComponent.h"
#include "Controller/PlayerCombatController.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Core/GameMode/CombatGameMode.h"

#include "UI/PlayerHUD.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWPlayerStatusBar.h"
#include "Interface/Interactable.h"

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

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
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

#pragma region Find Resource
	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CamShakeOnAttackFinder(TEXT("/Game/02-BP/CameraShake/BP_CameraShake_OnAttack.BP_CameraShake_OnAttack_C"));
	if (CamShakeOnAttackFinder.Succeeded())
		CameraShakeOnAttack = CamShakeOnAttackFinder.Class;

	static ConstructorHelpers::FClassFinder<UCameraShakeBase> CamShakeOnHitFinder(TEXT("/Game/02-BP/CameraShake/BP_CameraShake_OnHit.BP_CameraShake_OnHit_C"));
	if (CamShakeOnHitFinder.Succeeded())
		CameraShakeOnHit = CamShakeOnHitFinder.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InteractWidgetFinder(TEXT("/Game/06-UI/WBP_InteractIndicator.WBP_InteractIndicator_C"));
	if (InteractWidgetFinder.Succeeded())
		InteractWidget->SetWidgetClass(InteractWidgetFinder.Class);
#pragma endregion
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 플레이어 데이터 받아오기
	UPlayerManager* PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	StatComp->Init(PlayerManager->GetPlayerStat());
	StatComp->StartStaminaRecovery();

	EquipComp->Init();
	ActionComp->Init(GetMesh()->GetAnimInstance());

	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = WalkSpeed;

	if (APlayerCombatController* CombatController = Cast<APlayerCombatController>(GetController()))
	{
		APlayerHUD* PlayerHUD = CombatController->GetHUD<APlayerHUD>();
		ensure(PlayerHUD);

		UUWPlayerHUD* PlayerUI = Cast<UUWPlayerHUD>(PlayerHUD->GetPlayerUI());
		UUWPlayerStatusBar* StatusBar = PlayerUI->GetPlayerStatusBar();
		
		StatusBar->SetHealthBarPercent(StatComp->GetResourceValue(ECharacterResourceType::HEALTH), StatComp->GetResourceMaxValue(ECharacterResourceType::HEALTH));
		StatusBar->SetStaminaBarPercent(StatComp->GetResourceValue(ECharacterResourceType::STAMINA), StatComp->GetResourceMaxValue(ECharacterResourceType::STAMINA));
		StatusBar->SetSkillBarPercent(StatComp->GetResourceValue(ECharacterResourceType::SKILL), StatComp->GetResourceMaxValue(ECharacterResourceType::SKILL));

		// UI 이벤트 바인딩
		StatComp->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(StatusBar, &UUWPlayerStatusBar::SetHealthBarPercent);
		StatComp->GetResourceEvent(ECharacterResourceType::STAMINA).AddUObject(StatusBar, &UUWPlayerStatusBar::SetStaminaBarPercent);
		StatComp->GetResourceEvent(ECharacterResourceType::SKILL).AddUObject(StatusBar, &UUWPlayerStatusBar::SetSkillBarPercent);
	}

	InteractWidget->SetHiddenInGame(true);
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 액터 제거 시, 컴포넌트도 정리
	StatComp->Clear();
	ActionComp->Clear();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SmoothRotateToInputDir(DeltaTime);
	CheckInteractable();
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

	StatComp->TakeDamage(_hitInfo.Damage, 
		[this]() 
		{
			ActionComp->PlayHitAction(StatComp->IsDead());
			ShakeCamera(CameraShakeOnHit); 
		}
	);

	if (StatComp->IsDead())
		OnDead();
}

bool APlayerCharacter::IsDead()
{
	return StatComp->IsDead();
}

void APlayerCharacter::OnDead()
{
	// 플레이어 사망 후 처리
	// 플레이어 사망 이벤트 발행
	FStageEventContext Context;
	Context.Target = this;

	ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->PublishEvent(EStageEvent::PLAYER_DEAD, Context);
}

void APlayerCharacter::HandleAttackNotify(uint8 _opt)
{
	TArray<FHitResult> HitResults;
	bool IsHit = ActionComp->TraceAttack(_opt, HitResults);
	
	if (IsHit)
	{
		uint16 Damage = CalculateBaseDamage();
		bool bIsCritical;
		
		for (FHitResult& Hit : HitResults)
		{
			IHitable* Hitable = Cast<IHitable>(Hit.GetActor());
			if (Hitable == nullptr)
				continue;

			bIsCritical |= CalculateCritical(Damage);

			FHitInfo HitInfo;
			HitInfo.Damage = Damage;
			HitInfo.StaggerDamage = ActionComp->GetAttackActionStaggerDamage();
			HitInfo.KnockBackStrength = ActionComp->GetAttackActionKnockBack(_opt);
			HitInfo.Attacker = this;
			HitInfo.HitResult = &Hit;

			Hitable->HitBy(HitInfo);
		}

		ShakeCamera(CameraShakeOnAttack, bIsCritical ? 1.0f : 0.1f);
	}
}

uint16 APlayerCharacter::CalculateBaseDamage()
{
	return StatComp->GetStat(ECharacterStatType::ATTACK) * ActionComp->GetAttackActionDamagePer() * 0.01f;
}

bool APlayerCharacter::CalculateCritical(uint16& _damage)
{
	uint32 critial = FMath::Rand() % 100;

	bool bIsCritical = critial <= StatComp->GetStat(ECharacterStatType::CRITICAL_PERCENT);
	if (bIsCritical)
		_damage *= (1.0f + StatComp->GetStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT) * 0.01f);

	return bIsCritical;
}

void APlayerCharacter::ApplyEffect(TObjectPtr<UEffectData> _effectData)
{
	StatComp->ApplyEffect(_effectData);
}

void APlayerCharacter::ShakeCamera(TSubclassOf<UCameraShakeBase> _shakeClass, float _scale)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return;

	PlayerController->ClientStartCameraShake(_shakeClass, _scale);
}

#pragma region Interaction

void APlayerCharacter::CheckInteractable()
{
	FHitResult HitResult;

	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 500.0f;

	bool IsHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start, End,
		FVector(20.0f, 20.0f, 20.0f),
		GetActorForwardVector().Rotation(),
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel6),
		false,
		{ this },
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (IsHit == false)
	{
		InteractWidget->SetHiddenInGame(true);
		return;
	}

	if (IInteractable* Interactable = Cast<IInteractable>(HitResult.GetActor()))
	{
		CurInteractable = Interactable;
		InteractWidget->SetHiddenInGame(false);
		InteractWidget->SetWorldLocation(HitResult.GetActor()->GetActorLocation());
	}
}

void APlayerCharacter::Interact()
{
	if (nullptr == CurInteractable)
		return;

	CurInteractable->Interact();
}

#pragma endregion
