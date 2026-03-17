// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWPlayerHUD.h"
#include "Components/Button.h"

#include "Define/Enum.h"

void UUWNonCombatHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	MaintenanceButton->OnClicked.AddDynamic(this, &UUWNonCombatHUD::ClickMaintenanceButton);
	InventoryButton->OnClicked.AddDynamic(this, &UUWNonCombatHUD::ClickInventoryButton);
}

void UUWNonCombatHUD::ClickMaintenanceButton()
{
	OnClickShortCutButton.ExecuteIfBound(EShortCutType::TAB);
}

void UUWNonCombatHUD::ClickInventoryButton()
{
	OnClickShortCutButton.ExecuteIfBound(EShortCutType::KEY_I);
}
