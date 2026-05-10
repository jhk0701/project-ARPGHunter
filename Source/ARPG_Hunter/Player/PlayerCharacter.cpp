// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Damage.h" // 플레이어가 데미지를 줬을 경우, 몬스터 AI가 감지할 수 있게 이벤트 발행

#include "Interface/Interactable.h"
#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager/PlayerManager.h"
#include "Core/GameMode/Combat/CombatGameMode.h"
#include "Controller/Player/PlayerCombatController.h"
#include "Component/Stat/StatComponent.h"
#include "Component/Action/Player/PlayerActionComponent.h"
#include "Player/Equipment/Equipment.h"
#include "Player/SkillDevelop/SkillDevelop.h"
#include "Data/ItemData.h"
#include "Item/Item.h"
#include "AI/Sense/AISense_PlayerAction.h" // 플레이어가 특정 동작을 했음을 AI들에게 이벤트 발행
#include "AI/Sense/AISenseEvent_PlayerAction.h"

#include "UI/HUD/Combat/CombatHUD.h"
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

	StimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliComp"));
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

	ActorGroup = EActorGroup::FRIENDLY;
	SetEnemyCollisionChannel(ECC_GameTraceChannel4); // Monster Collision Channel
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
	InitStat(PlayerManager); 
	// 플레이어 장비 부착 스켈레탈 컴포넌트 초기화
	InitEquipment(PlayerManager); 
	// 무기에 따른 애니메이션 및 액션 초기화
	InitAction(PlayerManager); 
	// UI 초기화
	InitUI(); 

	// AI Perception Stimuli Source 업데이트
	StimuliSourceComp->RegisterWithPerceptionSystem();

	// 기타 수치 조절
	if (TObjectPtr<UCharacterMovementComponent> CharMove = Cast<UCharacterMovementComponent>(GetMovementComponent()))
		CharMove->MaxWalkSpeed = WalkSpeed;
}

void APlayerCharacter::InitStat(UPlayerManager* _pm)
{
	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
	if (false == Stat.IsValid())
		return;

	Stat->Init(_pm->GetStat(), _pm->GetEquipmentStat());
	Stat->StartStaminaRecovery();
}

void APlayerCharacter::InitEquipment(UPlayerManager* _pm)
{
	TWeakObjectPtr<UEquipment> Equipment = _pm->GetEquipment();
	if (Equipment.IsValid() == false)
		return;

	for (uint8 i = 0; i < static_cast<uint8>(EEquipmentType::END); ++i)
	{
		EEquipmentType Type = static_cast<EEquipmentType>(i);
		UpdateEquipment(Type, Equipment->GetEquipment(Type));
	}

	Equipment->OnEquipmentChanged.AddUObject(this, &APlayerCharacter::UpdateEquipment);
}

void APlayerCharacter::InitAction(UPlayerManager* _pm)
{
	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
	if (false == Stat.IsValid())
		return;

	TObjectPtr<UPlayerActionComponent> PlayerActionComp = GetActionComp<UPlayerActionComponent>();
	if (nullptr == PlayerActionComp)
		return;

	TObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();

	PlayerActionComp->Init({
			_pm->GetWeaponConfig(),
			AnimInst,
			MapEquipmentMeshComp[EEquipmentType::WEAPON],
			_pm->GetSkillDevelop()->GetSkillSelectPtr()
		});
	PlayerActionComp->StaminaUsagePredicate.BindUObject(Stat.Get(), &UStatComponent::TryUseStamina);

	AnimInst->OnMontageEnded.AddUniqueDynamic(this, &APlayerCharacter::OnMontageEnded);
}

void APlayerCharacter::InitUI()
{
	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();
	if (false == Stat.IsValid())
		return;

	TObjectPtr<UPlayerActionComponent> PlayerActionComp = GetActionComp<UPlayerActionComponent>();
	if (nullptr == PlayerActionComp)
		return;

	TObjectPtr<APlayerCombatController> CombatController = Cast<APlayerCombatController>(GetController());
	if (nullptr == CombatController)
		return;

	TObjectPtr<ACombatHUD> CombatHUD = CombatController->GetHUD<ACombatHUD>();
	check(CombatHUD);

	TObjectPtr<UUWPlayerStatusBar> StatusBarUI = CombatHUD->GetPlayerUI()->GetPlayerStatusBar();

	StatusBarUI->SetHealthBarPercent(Stat->GetResourceValue(ECharacterResourceType::HEALTH), Stat->GetResourceMaxValue(ECharacterResourceType::HEALTH));
	StatusBarUI->SetStaminaBarPercent(Stat->GetResourceValue(ECharacterResourceType::STAMINA), Stat->GetResourceMaxValue(ECharacterResourceType::STAMINA));
	StatusBarUI->SetSkillBarPercent(Stat->GetResourceValue(ECharacterResourceType::SKILL), Stat->GetResourceMaxValue(ECharacterResourceType::SKILL));

	// HPBar UI 이벤트 바인딩
	Stat->GetResourceEvent(ECharacterResourceType::HEALTH).AddUObject(StatusBarUI, &UUWPlayerStatusBar::SetHealthBarPercent);
	Stat->GetResourceEvent(ECharacterResourceType::STAMINA).AddUObject(StatusBarUI, &UUWPlayerStatusBar::SetStaminaBarPercent);
	Stat->GetResourceEvent(ECharacterResourceType::SKILL).AddUObject(StatusBarUI, &UUWPlayerStatusBar::SetSkillBarPercent);

	Stat->OnEffectRegistered.AddUObject(StatusBarUI, &UUWPlayerStatusBar::RegisterStatEffect);
	Stat->OnEffectRemoved.AddUObject(StatusBarUI, &UUWPlayerStatusBar::RemoveStatEffect);

	// 콤보 연계 UI 이벤트 바인딩
	TObjectPtr<UUWActionGuide> ActionGuideUI = CombatHUD->GetPlayerUI()->GetActionGuide();
	PlayerActionComp->OnActionUpdated.BindUObject(ActionGuideUI, &UUWActionGuide::SetActionInfo);
	PlayerActionComp->ResetAction();

	InteractWidget->SetHiddenInGame(true);
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
	if (IsDead() || GetStatComp()->IsStaggering())
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
	if (false == ActionComp->IsValid() || IsDead())
		return;

	// ActionComp에 회피 액션 사용을 위한 조건 전달
	bool bIsSuccess = GetActionComp<UPlayerActionComponent>()->PlayDodgeAction(InputDirection.SizeSquared() > 0);

	if (bIsSuccess)
		SetIgnoreInput(true);
}

void APlayerCharacter::Attack(EAttackType _eType)
{
	if (false == ActionComp->IsValid() || IsDead())
		return;

	bool bIsValid = GetActionComp<UPlayerActionComponent>()->PlayAttackAction(_eType);
	if (bIsValid) 
	{
		// 아이템 사용 자극 이벤트
		ReportPlayerActionEvent(static_cast<uint8>(EPlayerActionType::ATTACK), RangePerAction[EPlayerActionType::ATTACK]);
	}

	if (false == bIsValid || InputDirection.SquaredLength() > 0)
		return;

	// 주변 적 자동 조준
	// 이동 입력이 없을 때, 조준
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	bool bIsHit = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		Start, Start, AutoOrientToEnemyRadius,
		UEngineTypes::ConvertToTraceType(GetEnemyCollisionChannel()), false,
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
	if (false == ActionComp->IsValid() || IsDead())
		return;

	GetActionComp<UPlayerActionComponent>()->ProcessAttackEnd();
}

void APlayerCharacter::SetActionProcess(EActionProcess _eProcess)
{
	if (ActionComp->IsValid() == false)
		return;
	GetActionComp<UPlayerActionComponent>()->SetActionProcess(_eProcess);
}

void APlayerCharacter::HandleAttackNotify(uint8 _opt)
{
	if (IsDead())
		return;

	TWeakObjectPtr<APlayerCharacter> WeakThis(this);
	
	// 공격 로직 수행
	ActionComp->ProcessAttack(_opt, GetEnemyCollisionChannel(),
		[WeakThis, _opt](TArray<FHitResult>& _hitResults)
		{
			if (false == WeakThis.IsValid())
				return;

			TWeakObjectPtr<UStatComponent> Stat = WeakThis->GetStatComp();
			if (false == Stat.IsValid())
				return;

			bool bIsCritical = false;
			uint32 Damage = ACombatGameMode::CalculateAttack(
				Stat->GetStat(ECharacterStatType::ATTACK),
				WeakThis->GetActionComp<UPlayerActionComponent>()->GetAttackActionDamagePer(_opt)
			);

			for (FHitResult& Hit : _hitResults)
				bIsCritical |= WeakThis->HitTarget(Hit, Damage, _opt);

			// 공격 시, 카메라 셰이크
			WeakThis->ShakeCameraOnAttack(bIsCritical ? 1.0f : 0.75f);
		}
	);
}

bool APlayerCharacter::HitTarget(FHitResult& _hit, uint32 _damage, uint8 _opt)
{
	// 피격 대상들에게 피격 처리
	IHitable* Hitable = Cast<IHitable>(_hit.GetActor());
	if (nullptr == Hitable)
		return false;

	TWeakObjectPtr<UStatComponent> Stat = GetStatComp();

	FHitInfo HitInfo;
	HitInfo.bIsCriticalHit = ACombatGameMode::CalculateCritical(
		Stat->GetStat(ECharacterStatType::CRITICAL_PERCENT),
		Stat->GetStat(ECharacterStatType::CRITICAL_DAMAGE_PERCENT),
		_damage);
	HitInfo.Damage = _damage;
	HitInfo.StaggerDamage = GetActionComp<UPlayerActionComponent>()->GetAttackActionStaggerDamage(_opt);
	HitInfo.KnockBackStrength = GetActionComp<UPlayerActionComponent>()->GetAttackActionKnockBack(_opt);
	HitInfo.AttackType = GetActionComp<UPlayerActionComponent>()->GetAttackActionType();
	HitInfo.Attacker = this;
	HitInfo.HitResult = &_hit;

	Hitable->HitBy(HitInfo);

	// 맞은 몬스터에게 Damage Sense 유형 이벤트 발행
	UAISense_Damage::ReportDamageEvent(
		GetWorld(),
		_hit.GetActor(),
		this,
		static_cast<float>(_damage),
		GetActorLocation(),
		_hit.ImpactPoint
	);

	return HitInfo.bIsCriticalHit;
}

void APlayerCharacter::OnDead()
{
	// 플레이어 사망 후 처리
	Super::OnDead();

	// 플레이어 사망 이벤트 발행
	FStageEventContext Context;
	Context.Target = this;

	TObjectPtr<ACombatGameMode> GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->PublishEvent(EStageEvent::PLAYER_DEAD, Context);

	GetActionComp<UPlayerActionComponent>()->PlayDeadAction(); // 사망 애니메이션 실행
}

void APlayerCharacter::OnCharacterHit()
{
	Super::OnCharacterHit();

	GetActionComp<UPlayerActionComponent>()->PlayHitAction(); // hit 애니메이션 실행
	ShakeCamera(CameraShakeOnHit);
}

void APlayerCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	TObjectPtr<UPlayerActionComponent> PlayerAction = GetActionComp<UPlayerActionComponent>();

	if (PlayerAction->GetDodgeMontage() == Montage ||
		PlayerAction->GetHitMontage() == Montage)
		SetIgnoreInput(false);
}

void APlayerCharacter::UseQuickSlot(uint8 _index)
{
	if (IsDead())
		return;

	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TWeakObjectPtr<UConsumableItem> QuickSlotItem = PlayerManager->GetQuickSlotItem(_index);

	if (QuickSlotItem.IsValid() == false)
		return;

	// 아이템 사용 모션 재생
	GetActionComp<UPlayerActionComponent>()->PlayItemUsageAction();
	UsingQuickSlotIndex = _index;
}

void APlayerCharacter::HandleUseItemNotify()
{
	// 퀵슬롯 사용
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TWeakObjectPtr<UConsumableItem> Item = PlayerManager->GetQuickSlotItem(UsingQuickSlotIndex);
	if (false == Item.IsValid())
		return;

	TObjectPtr<UConsumableItemConfig> Config = Cast<UConsumableItemConfig>(Item->GetConfig());
	if (Config->VFX)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Config->VFX, GetActorLocation(), GetActorRotation(), true, EPSCPoolMethod::AutoRelease);

	PlayerManager->UseQuickSlotItem(UsingQuickSlotIndex, this);

	// 아이템 사용 자극 이벤트
	ReportPlayerActionEvent(static_cast<uint8>(EPlayerActionType::USE_ITEM), RangePerAction[EPlayerActionType::USE_ITEM]);
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

void APlayerCharacter::ReportPlayerActionEvent(uint8 _actionType, float _range)
{
	FAIPlayerActionStimulusEvent Event;
	Event.ActionType = _actionType;
	Event.Instigator = this;
	Event.Location = GetActorLocation();
	Event.Range = _range;

	UAISense_PlayerAction::ReportEvent(this, Event);
}
