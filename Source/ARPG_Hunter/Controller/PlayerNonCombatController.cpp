// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerNonCombatController.h"

#include "UI/NonCombatHUD.h"


void APlayerNonCombatController::ShortCut(EShortCutType _key)
{
	Super::ShortCut(_key);

	if(_key == EShortCutType::TAB)
	{
		if (TObjectPtr<ANonCombatHUD> HUD = Cast<ANonCombatHUD>(GetHUD())) 
			HUD->ShowMaintenanceUI();
	}
}
