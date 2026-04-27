// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/TestGameMode.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/QuickSlot.h"
#include "Player/Inventory.h"

void ATestGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 테스트 모드 강제 할당
	SetStageData(TEXT("0"));

	UPlayerManager* PM = GetGameInstance()->GetSubsystem<UPlayerManager>();

	uint8 idx = PM->AddItem(FName(TEXT("2001")), 5);
	PM->GetQuickSlot()->Register(0, PM->GetInventory()->GetItem(EItemType::CONSUMABLE, idx));
}