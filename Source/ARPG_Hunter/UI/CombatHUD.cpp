// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/CombatHUD.h"

#include "Core/GameMode/CombatGameMode.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/QuickSlot.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWStageResult.h"
#include "UI/UserWidget/UWQuickSlot.h"

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

	if (PlayerUIClass)
	{
		PlayerUI = CreateWidget<UUWCombatHUD>(GetWorld(), PlayerUIClass);
		if (PlayerUI) 
		{
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
			
			TObjectPtr<UQuickSlot> QuickSlot = PlayerManager->GetQuickSlot();
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
	}

	if (StageResultUIClass)
	{
		StageResultUI = CreateWidget<UUWStageResult>(GetWorld(), StageResultUIClass);

		if (TObjectPtr<ACombatGameMode> GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>())
			GameMode->OnGameEnd.AddUObject(this, &ACombatHUD::ShowResultUI);
	}

	if (DamageUIClass)
	{
		TObjectPtr<UObjectPoolManager> ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
		ObjectPool->Register(ADamageFont::StaticClass(), [this]() { return GetWorld()->SpawnActor(DamageUIClass); }, 10);
	}
}

void ACombatHUD::ShowResultUI(bool _bOnCleared, const FStageData* _stageData)
{
	TObjectPtr<UUWStageResult> ResultUI = Cast<UUWStageResult>(StageResultUI);

	ResultUI->Update(_bOnCleared, _stageData);
	ResultUI->OnClickReturnButton.BindLambda(
		[this]()
		{
			if (ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>())
				GameMode->BackToTown();
		}
	);

	ResultUI->ShowUI();
}
