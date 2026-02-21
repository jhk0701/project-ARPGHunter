// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Core/Subsystem/DataManager.h"
#include "Core/GameMode/CombatGameMode.h"
#include "Controller/PlayerCombatController.h"
#include "Component/StatComponent.h"
#include "Component/ActionComponent/PlayerActionComponent.h"
#include "Player/Equipment.h"
#include "Data/WeaponConfig.h"
#include "Data/ItemData.h"
#include "Item/Item.h"

#include "UI/CombatHUD.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWPlayerStatusBar.h"
#include "Interface/Interactable.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region Create Comp
	StatComp = CreateDefaultSubobject<UStatComponent>(TEXT("StatComp"));
	ActionComp = CreateDefaultSubobject<UPlayerActionComponent>(TEXT("ActionComp"));
	
	MapEquipmentMeshComp.Add(EEquipmentType::TOP, GetMesh());

	for (uint8 i = 0; i < static_cast<uint8>(EEquipmentType::END); ++i)
	{
		EEquipmentType Type = static_cast<EEquipmentType>(i);
		if (Type == EEquipmentType::TOP)
			continue;

		TObjectPtr<USkeletalMeshComponent>& MeshComp = MapEquipmentMeshComp.Add(Type, CreateDefaultSubobject<USkeletalMeshComponent>(*FString::Printf(TEXT("%sMesh"), *EnumToString(Type))));
		
		if (Type == EEquipmentType::WEAPON)
			MeshComp->SetupAttachment(MapEquipmentMeshComp[EEquipmentType::TOP], FName(TEXT("socket_hand_r")));
		else
		{
			MeshComp->SetupAttachment(MapEquipmentMeshComp[EEquipmentType::TOP]);
			MeshComp->SetLeaderPoseComponent(MapEquipmentMeshComp[EEquipmentType::TOP]);
		}
	}

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(GetRootComponent());
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
#pragma endregion

#pragma region Init Comp
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
	// TODO : 플레이어 저장 데이터 기반으로 변경
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	
	StatComp->Init(PlayerManager->GetStat(), PlayerManager->GetEquipmentStat());
	StatComp->StartStaminaRecovery();
	StatComp->OnDead.AddUObject(this, &APlayerCharacter::OnDead);

	TObjectPtr<UEquipment> Equipment = PlayerManager->GetEquipment();
	InitEquipment(Equipment);
	Equipment->OnEquipmentChanged.AddUObject(this, &APlayerCharacter::UpdateEquipment);
	
	ActionComp->Init(DataManager->GetWeaponConfig(EWeaponType::SWORD), GetMesh()->GetAnimInstance(), MapEquipmentMeshComp[EEquipmentType::WEAPON]);

	if (TObjectPtr<UCharacterMovementComponent> CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = WalkSpeed;

	if (TObjectPtr<APlayerCombatController> CombatController = Cast<APlayerCombatController>(GetController()))
	{
		TObjectPtr<ACombatHUD> CombatHUD = CombatController->GetHUD<ACombatHUD>();
		check(CombatHUD);

		TObjectPtr<UUWPlayerStatusBar> StatusBarUI = CombatHUD->GetPlayerUI()->GetPlayerStatusBar();
		
		StatusBarUI->SetHealthBarPercent(StatComp->GetResourceValue(ECharacterResourceType::HEALTH), StatComp->GetResourceMaxValue(ECharacterResourceType::HEALTH));
		StatusBarUI->SetStaminaBarPercent(StatComp->GetResourceValue(ECharacterResourceType::STAMINA), StatComp->GetResourceMaxValue(ECharacterResourceType::STAMINA));
		StatusBarUI->SetSkillBarPercent(StatComp->GetResourceValue(ECharacterResourceType::SKILL), StatComp->GetResourceMaxValue(ECharacterResourceType::SKILL));

		// HPBar UI 이벤트 바인딩
		StatComp->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(StatusBarUI, &UUWPlayerStatusBar::SetHealthBarPercent);
		StatComp->GetResourceEvent(ECharacterResourceType::STAMINA).AddUObject(StatusBarUI, &UUWPlayerStatusBar::SetStaminaBarPercent);
		StatComp->GetResourceEvent(ECharacterResourceType::SKILL).AddUObject(StatusBarUI, &UUWPlayerStatusBar::SetSkillBarPercent);
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

void APlayerCharacter::InitEquipment(TObjectPtr<UEquipment> _equipment)
{
	for (uint8 i = 0; i < static_cast<uint8>(EEquipmentType::END); ++i)
	{
		EEquipmentType Type = static_cast<EEquipmentType>(i);
		UpdateEquipment(Type, _equipment->GetEquipment(Type));
	}
	
}

void APlayerCharacter::UpdateEquipment(EEquipmentType _type, TWeakObjectPtr<UEquipmentItem> _equipment)
{
	if (_equipment == nullptr)
	{
		TObjectPtr<USkeletalMesh> DefaultMesh = GetGameInstance()->GetSubsystem<UPlayerManager>()->GetDefaultMesh(_type);
		MapEquipmentMeshComp[_type]->SetSkeletalMesh(DefaultMesh);
		return;
	}

	if (TObjectPtr<UEquipmentItemConfig> Config = Cast<UEquipmentItemConfig>(_equipment->GetConfig())) 
		MapEquipmentMeshComp[_type]->SetSkeletalMesh(Config->Mesh);
	else
		MapEquipmentMeshComp[_type]->SetSkeletalMesh(nullptr);
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

	uint32 Damage = _hitInfo.Damage;
	AdjustDefense(Damage);

	StatComp->TakeDamage(Damage,
		[this]() 
		{
			ActionComp->PlayHitAction(); // hit 애니메이션 실행
			ShakeCamera(CameraShakeOnHit); 
		}
	);
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

	TObjectPtr<ACombatGameMode> GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->PublishEvent(EStageEvent::PLAYER_DEAD, Context);

	ActionComp->PlayDeadAction(); // 사망 애니메이션 실행
}

void APlayerCharacter::HandleAttackNotify(uint8 _opt)
{
	TWeakObjectPtr<APlayerCharacter> WeakThis(this);
	TWeakObjectPtr<UPlayerActionComponent> WeakActionComp(ActionComp);

	ActionComp->ProcessAttack(_opt, ECC_GameTraceChannel4,
		[WeakThis, WeakActionComp, _opt](TArray<FHitResult>& _hitResults)
		{
			if (WeakThis.IsValid() == false || WeakActionComp.IsValid() == false)
				return;

			uint32 Damage = WeakThis->CalculateBaseDamage();
			bool bIsCritical = false;

			for (FHitResult& Hit : _hitResults)
			{
				IHitable* Hitable = Cast<IHitable>(Hit.GetActor());
				if (Hitable == nullptr)
					continue;

				FHitInfo HitInfo;
				HitInfo.bIsCriticalHit = WeakThis->CalculateCritical(Damage);
				HitInfo.Damage = Damage;
				HitInfo.StaggerDamage = WeakActionComp->GetAttackActionStaggerDamage();
				HitInfo.KnockBackStrength = WeakActionComp->GetAttackActionKnockBack(_opt);
				HitInfo.AttackType = WeakActionComp->GetAttackActionType();
				HitInfo.Attacker = WeakThis;
				HitInfo.HitResult = &Hit;

				bIsCritical |= HitInfo.bIsCriticalHit;

				Hitable->HitBy(HitInfo);
			}

			WeakThis->ShakeCameraOnAttack(bIsCritical ? 1.0f : 0.5f);
		}
	);
}

uint32 APlayerCharacter::CalculateBaseDamage()
{
	return StatComp->GetStat(ECharacterStatType::ATTACK) * ActionComp->GetAttackActionDamagePer() * 0.01f;
}

bool APlayerCharacter::CalculateCritical(uint32& _outDamage)
{
	uint32 critial = FMath::Rand() % 100;

	bool bIsCritical = critial <= StatComp->GetStat(ECharacterStatType::CRITICAL_PERCENT);
	if (bIsCritical)
		_outDamage *= (1.0f + StatComp->GetStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT) * 0.01f);

	return bIsCritical;
}

void APlayerCharacter::AdjustDefense(uint32& _outDamage)
{
	uint32 DefensedValue = StatComp->GetStat(ECharacterStatType::DEFENSE) / 3;
	
	if (_outDamage >= DefensedValue)
		_outDamage -= DefensedValue;
	else
		_outDamage = 0;
}

void APlayerCharacter::ApplyEffect(TObjectPtr<UEffectData> _effectData)
{
	if (StatComp->IsDead())
		return;

	StatComp->ApplyEffect(_effectData);
}

void APlayerCharacter::UseQuickSlot(uint8 _index)
{
	if (StatComp->IsDead())
		return;

	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TWeakObjectPtr<UConsumableItem> QuickSlotItem = PlayerManager->GetQuickSlotItem(_index);

	if (QuickSlotItem.IsValid() == false)
		return;

	// 아이템 사용 모션 재생
	ActionComp->PlayItemUsageAction();
	UsingQuickSlotIndex = _index;
}

void APlayerCharacter::HandleUseItemNotify()
{
	// 퀵슬롯 사용
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TWeakObjectPtr<UConsumableItem> Item = PlayerManager->GetQuickSlotItem(UsingQuickSlotIndex);
	if (Item.IsValid()) 
	{
		TObjectPtr<UConsumableItemConfig> Config = Cast<UConsumableItemConfig>(Item->GetConfig());
		if (Config->VFX)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Config->VFX, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);
		}

		PlayerManager->UseQuickSlotItem(UsingQuickSlotIndex, this);
	}
}

void APlayerCharacter::ShakeCamera(TSubclassOf<UCameraShakeBase> _shakeClass, float _scale)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
		return;

	PlayerController->ClientStartCameraShake(_shakeClass, _scale);
}

void APlayerCharacter::ShakeCameraOnAttack(float _scale)
{
	ShakeCamera(CameraShakeOnAttack, _scale);
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
