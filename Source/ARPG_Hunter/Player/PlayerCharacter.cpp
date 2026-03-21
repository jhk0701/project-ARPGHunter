// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"

#include "Interface/Interactable.h"
#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Core/GameMode/CombatGameMode.h"
#include "Controller/PlayerCombatController.h"
#include "Component/StatComponent.h"
#include "Component/ActionComponent/PlayerActionComponent.h"
#include "Player/Equipment.h"
#include "Player/SkillDevelop.h"
#include "Data/ItemData.h"
#include "Item/Item.h"

#include "UI/CombatHUD.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWPlayerStatusBar.h"
#include "UI/UserWidget/UWActionGuide.h"
#include "UI/UserWidget/UWInteractionIndicator.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponSocketOnCombat = FName(TEXT("socket_hand_r"));
	WeaponSocketOnNonCombat = FName(TEXT("socket_weapon_container"));

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
			MeshComp->SetupAttachment(MapEquipmentMeshComp[EEquipmentType::TOP], WeaponSocketOnCombat);
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
	
	Init();
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 액터 제거 시, 컴포넌트도 정리
	StatComp->Clear();
	ActionComp->Clear();

	GetWorld()->GetTimerManager().ClearTimer(CameraAnimTimer);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SmoothRotateToInputDir(DeltaTime);
	CheckInteractable();
}

void APlayerCharacter::Init()
{
	// 플레이어 데이터 받아오기
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

	// 스탯 초기화
	StatComp->Init(PlayerManager->GetStat(), PlayerManager->GetEquipmentStat());
	StatComp->StartStaminaRecovery();
	StatComp->OnDead.AddUObject(this, &APlayerCharacter::OnDead);

	// 장비 초기화
	TWeakObjectPtr<UEquipment> Equipment = PlayerManager->GetEquipment();
	InitEquipment(Equipment);
	Equipment->OnEquipmentChanged.AddUObject(this, &APlayerCharacter::UpdateEquipment);

	// 무기에 따른 애니메이션 및 액션 초기화
	TObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();

	ActionComp->Init({
			PlayerManager->GetWeaponConfig(),
			AnimInst,
			MapEquipmentMeshComp[EEquipmentType::WEAPON],
			PlayerManager->GetSkillDevelop()->GetSkillSelectPtr()
		});
	ActionComp->StaminaUsagePredicate.BindUObject(StatComp, &UStatComponent::TryUseStamina);

	AnimInst->OnMontageEnded.AddUniqueDynamic(this, &APlayerCharacter::OnMontageEnded);

	// UI 초기화
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

		StatComp->OnEffectRegistered.AddUObject(StatusBarUI, &UUWPlayerStatusBar::RegisterStatEffect);
		StatComp->OnEffectRemoved.AddUObject(StatusBarUI, &UUWPlayerStatusBar::RemoveStatEffect);

		TObjectPtr<UUWActionGuide> ActionGuideUI = CombatHUD->GetPlayerUI()->GetActionGuide();
		ActionComp->OnActionUpdated.BindUObject(ActionGuideUI, &UUWActionGuide::SetActionInfo);
		ActionComp->ResetAction();
	}

	InteractWidget->SetHiddenInGame(true);

	// 기타 수치 조절
	if (TObjectPtr<UCharacterMovementComponent> CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::InitEquipment(TWeakObjectPtr<UEquipment> _equipment)
{
	if (_equipment.IsValid() == false)
		return;

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
	// 이동 금지 상황일 땐, 돌리지 않음
	if (InputDirection.SizeSquared() <= 0 || bIgnoreMoveInput)
		return;

	FRotator TargetRot = GetActorRotation();
	TargetRot.Yaw = GetControlRotation().Yaw + FMath::RadiansToDegrees(FMath::Atan2(InputDirection.Y, InputDirection.X));

	SetActorRotation(FQuat::Slerp(GetActorQuat(), TargetRot.Quaternion(), RotateSpeedToInputDir * DeltaTime));
}

void APlayerCharacter::SetIsSprint(bool _isSprint)
{
	if(StatComp->IsStaggering())
		bIsSprint = false;
	else
		bIsSprint = _isSprint;

	if (UCharacterMovementComponent* CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = bIsSprint ? SprintSpeed : WalkSpeed;
}

void APlayerCharacter::SetIsCombat(bool _bIsCombat)
{
	bIsCombat = _bIsCombat;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	MapEquipmentMeshComp[EEquipmentType::WEAPON]->AttachToComponent(
		MapEquipmentMeshComp[EEquipmentType::TOP],
		AttachmentRules,
		bIsCombat ? WeaponSocketOnCombat : WeaponSocketOnNonCombat);
}

void APlayerCharacter::Dodge()
{
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	// ActionComp에 회피 액션 사용을 위한 조건 전달
	bool bIsSuccess = ActionComp->PlayDodgeAction(InputDirection.SizeSquared() > 0);

	if (bIsSuccess)
		SetIgnoreInput(true);
}

void APlayerCharacter::Attack(EAttackType _eType)
{
	if (ActionComp->IsValid() == false || StatComp->IsDead())
		return;

	bool bIsValid = ActionComp->PlayAttackAction(_eType);
	if (bIsValid == false || InputDirection.SquaredLength() > 0)
		return;

	// 주변 적 자동 조준
	// 이동 입력이 없을 때, 조준
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start, Start, AutoOrientToEnemyRadius,
		UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4), false,
		{}, EDrawDebugTrace::None, 
		HitResult, true);

	if(bIsHit)
	{
		AActor* Target = HitResult.GetActor();
		if(IHitable* Hitable = Cast<IHitable>(Target))
		{
			FVector Dir = Target->GetActorLocation() - GetActorLocation();
			Dir.Z = 0;
			Dir.Normalize();

			SetActorRotation(Dir.ToOrientationQuat());
		}
	}
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

	uint32 Damage = _hitInfo.bIgnoreDefense ? 
		_hitInfo.Damage : 
		ACombatGameMode::CalculateDefense(_hitInfo.Damage, StatComp->GetStat(ECharacterStatType::DEFENSE));

	StatComp->TakeDamage(Damage,
		[this]() 
		{
			ActionComp->PlayHitAction(); // hit 애니메이션 실행
			ShakeCamera(CameraShakeOnHit); 
		}
	);
}

void APlayerCharacter::HandleAttackNotify(uint8 _opt)
{
	TWeakObjectPtr<APlayerCharacter> WeakThis(this);
		
	uint8 OptionIdx = _opt;
	ActionComp->ProcessAttack(_opt, ECC_GameTraceChannel4,
		[WeakThis, OptionIdx](TArray<FHitResult>& _hitResults)
		{
			if (WeakThis.IsValid() == false)
				return;

			UStatComponent* Stat = WeakThis->StatComp;
			UPlayerActionComponent* Action = WeakThis->ActionComp;
			uint32 BaseDamage = ACombatGameMode::CalculateAttack(
				Stat->GetStat(ECharacterStatType::ATTACK),
				Action->GetAttackActionDamagePer(OptionIdx)
			);

			bool bIsCritical = false;
			for (FHitResult& Hit : _hitResults)
			{
				IHitable* Hitable = Cast<IHitable>(Hit.GetActor());
				if (Hitable == nullptr)
					continue;

				uint32 Damage = BaseDamage;

				FHitInfo HitInfo;
				HitInfo.bIsCriticalHit = ACombatGameMode::CalculateCritical(
					Stat->GetStat(ECharacterStatType::CRITICAL_PERCENT),
					Stat->GetStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT),
					Damage);
				HitInfo.Damage = Damage;
				HitInfo.StaggerDamage = Action->GetAttackActionStaggerDamage(OptionIdx);
				HitInfo.KnockBackStrength = Action->GetAttackActionKnockBack(OptionIdx);
				HitInfo.AttackType = Action->GetAttackActionType();
				HitInfo.Attacker = WeakThis;
				HitInfo.HitResult = &Hit;

				bIsCritical |= HitInfo.bIsCriticalHit;

				Hitable->HitBy(HitInfo);
			}

			WeakThis->ShakeCameraOnAttack(bIsCritical ? 1.0f : 0.75f);
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

void APlayerCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (ActionComp->GetDodgeMontage() == Montage || ActionComp->GetHitMontage() == Montage)
		SetIgnoreInput(false);
}

void APlayerCharacter::ApplyEffect(const FApplyEffectParam& _param)
{
	if (StatComp->IsDead())
		return;

	StatComp->ApplyEffect(_param);
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

#pragma region Interaction

void APlayerCharacter::CheckInteractable()
{
	FHitResult HitResult;

	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * InteractionRange;

	bool IsHit = UKismetSystemLibrary::BoxTraceSingle(
		GetWorld(),
		Start, End,
		FVector(InteractionSize),
		CameraComp->GetForwardVector().Rotation(),
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

		if (UUWInteractionIndicator* Indicator = Cast<UUWInteractionIndicator>(InteractWidget->GetWidget()))
		{
			Indicator->SetText(CurInteractable->GetInteractName());
		}

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

#pragma region Camera

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

void APlayerCharacter::SetCameraLag(bool _bIsEnable, float _speed)
{
	SpringArmComp->bEnableCameraLag = _bIsEnable;
	SpringArmComp->CameraLagSpeed = _speed;
	SpringArmComp->bDoCollisionTest = !_bIsEnable;
}

void APlayerCharacter::PlayCameraAnim(TObjectPtr<UCurveVector> _animCurve, float _duration)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	// 중복 실행 확인
	if (TimerManager.IsTimerActive(CameraAnimTimer))
		TimerManager.ClearTimer(CameraAnimTimer);

	// 카메라 애니메이션 시작
	CameraAnimCurve = _animCurve;
	CameraAnimElapsedTime = 0.0f;

	TimerManager.SetTimer(
		CameraAnimTimer, 
		this, &APlayerCharacter::ProgressCameraCurve,
		CameraAnimInterval, 
		true
	);
}

void APlayerCharacter::ProgressCameraCurve()
{
	if (nullptr == CameraAnimCurve || CameraAnimTimer.IsValid() == false)
		return;

	CameraAnimElapsedTime += CameraAnimInterval;

	float Percent = FMath::Min(1.0f, CameraAnimElapsedTime / CameraAnimDuration);
	FVector CamRelativeLoc = CameraAnimCurve->GetVectorValue(Percent);

	// 카메라 애니메이션 적용
	CameraComp->SetRelativeLocation(CamRelativeLoc);

	// 완료
	if (CameraAnimElapsedTime >= CameraAnimDuration)
	{
		// 반복 호출 종료
		GetWorld()->GetTimerManager().ClearTimer(CameraAnimTimer);
		CameraAnimCurve = nullptr;
	}
}

#pragma endregion

void APlayerCharacter::SetIgnoreInput(bool _bIgnoreMoveInput)
{
	bIgnoreMoveInput = _bIgnoreMoveInput;
	Controller->SetIgnoreMoveInput(bIgnoreMoveInput);
}
