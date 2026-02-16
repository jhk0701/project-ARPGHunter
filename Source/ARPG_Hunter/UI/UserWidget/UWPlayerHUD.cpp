// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWPlayerHUD.h"
#include "Components/Button.h"


void UUWNonCombatHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	MaintenanceButton->OnClicked.AddDynamic(this, &UUWNonCombatHUD::ClickMaintenanceButton);
}

void UUWNonCombatHUD::ClickMaintenanceButton()
{
	OnClickMaintenanceButton.ExecuteIfBound();
}
