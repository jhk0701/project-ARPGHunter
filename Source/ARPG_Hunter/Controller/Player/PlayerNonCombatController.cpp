// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Player/PlayerNonCombatController.h"

#include "Define/Enum.h"
#include "Player/PlayerCharacter.h"
#include "UI/HUD/NonCombat/NonCombatHUD.h"


void APlayerNonCombatController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);

	GetControlledPlayer()->SetIsCombat(false);
}

void APlayerNonCombatController::ShortCut(EShortCutType _key)
{
	Super::ShortCut(_key);

	if (TObjectPtr<ANonCombatHUD> HUD = Cast<ANonCombatHUD>(GetHUD()))
	{
		if (_key == EShortCutType::TAB)
			HUD->ToggleMaintenanceUI();
		else if (_key == EShortCutType::KEY_I)
			HUD->ToggleInventoryUI();
		else if (_key == EShortCutType::KEY_K)
			HUD->ToggleSkillDevelopUI();
	}
}
