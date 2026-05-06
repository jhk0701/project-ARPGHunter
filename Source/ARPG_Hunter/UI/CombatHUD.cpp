// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CombatHUD.h"

#include "Core/GameMode/CombatGameMode.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/QuickSlot.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWStageResult.h"
#include "UI/UserWidget/UWQuickSlot.h"
#include "UI/UserWidget/UWGameMenu.h"

#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "UI/Actor/DamageFont.h"

ACombatHUD::ACombatHUD()
{
	// 플레이어 HUD
	static ConstructorHelpers::FClassFinder<UUWCombatHUD> PlayerUIFinder(TEXT("/Game/06-UI/HUD/WBP_CombatHUD.WBP_CombatHUD_C"));
	if (PlayerUIFinder.Succeeded())
		PlayerUIClass = PlayerUIFinder.Class;

	// 스테이지 결과 UI
	static ConstructorHelpers::FClassFinder<UUWStageResult> StageResultUIFinder(TEXT("/Game/06-UI/WBP_StageResult.WBP_StageResult_C"));
	if (StageResultUIFinder.Succeeded())
		StageResultUIClass = StageResultUIFinder.Class;

	static ConstructorHelpers::FClassFinder<AActor> DamageUIFinder(TEXT("/Game/02-BP/UI/BP_DamageFont.BP_DamageFont_C"));
	if (DamageUIFinder.Succeeded())
		DamageUIClass = DamageUIFinder.Class;
}

void ACombatHUD::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerUI();
	InitStageResultUI();
	InitDamageUI();
}

void ACombatHUD::InitMenuUI()
{
	Super::InitMenuUI();

	TWeakObjectPtr<UUWPopUp> MenuUIInst = GetGameMenuUI();
	if (false == MenuUIInst.IsValid())
		return;

	TObjectPtr<UUWGameMenu> MenuUI = Cast<UUWGameMenu>(MenuUIInst);
	MenuUI->OnReturnClicked.BindLambda(
		[this]()
		{
			if (AARPGGameMode* GM = GetWorld()->GetAuthGameMode<AARPGGameMode>())
				GM->GoToTown();
		}
	);
}

void ACombatHUD::InitPlayerUI()
{
	if (false == TryCreateWidget<UUWCombatHUD>(PlayerUIClass, PlayerUI))
		return;

	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

	TWeakObjectPtr<UQuickSlot> QuickSlot = PlayerManager->GetQuickSlot();
	TObjectPtr<UUWQuickSlot> QuickSlotUI = PlayerUI->GetQuickSlot();
	QuickSlotUI->Init(QuickSlot->GetContainer());

	// 전투 관련 월드에서만 띄울 것이므로 AddWeakLambda로 바인딩
	QuickSlot->OnPostQuickSlotUsed.AddWeakLambda(this,
		[this](uint8 _quickSlotIdx, uint8 _inventoryIdx)
		{
			// 플레이어가 퀵슬롯 아이템 사용 시, 업데이트
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
			PlayerUI->GetQuickSlot()->SetQuickSlot(_quickSlotIdx, PlayerManager->GetQuickSlotItem(_quickSlotIdx));
		}
	);
	QuickSlot->OnQuickSlotChanged.AddWeakLambda(this,
		[this](uint8 _quickSlotIdx, TWeakObjectPtr<UConsumableItem> _item)
		{
			PlayerUI->GetQuickSlot()->SetQuickSlot(_quickSlotIdx, _item);
		}
	);

	PlayerUI->AddToViewport();
}

void ACombatHUD::InitStageResultUI()
{
	if (false == TryCreateWidget<UUWStageResult>(StageResultUIClass, StageResultUI))
		return;

	if (TObjectPtr<ACombatGameMode> GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>())
		GameMode->OnGameEnd.AddUObject(this, &ACombatHUD::ShowResultUI);
}

void ACombatHUD::InitDamageUI()
{
	if (nullptr == DamageUIClass)
		return;

	// 데미지 UI 오브젝트 풀링 등록
	TObjectPtr<UObjectPoolManager> ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	ObjectPool->Register(ADamageFont::StaticClass(), [this]() { return GetWorld()->SpawnActor(DamageUIClass); }, 10);
}

void ACombatHUD::ShowPlayerUI(bool _bIsShow)
{
	if (nullptr == PlayerUI)
		return;

	// 플레이어 UI는 이미 ViewPort에 추가된 상태로 유지할 것이니
	// SetVisibility로 제어할 것
	PlayerUI->SetVisibility(_bIsShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void ACombatHUD::ShowResultUI(bool _bOnCleared, const FStageData* _stageData)
{
	TObjectPtr<UUWStageResult> ResultUI = Cast<UUWStageResult>(StageResultUI);

	ResultUI->Update(_bOnCleared, _stageData);
	ResultUI->OnClickReturnButton.BindLambda(
		[this]()
		{
			if (ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>())
				GameMode->GoToTown();
		}
	);

	ResultUI->ShowUI();
}
