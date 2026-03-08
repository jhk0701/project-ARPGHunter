// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerNonCombatController.h"

#include "Define/Enum.h"
#include "Player/PlayerCharacter.h"
#include "UI/NonCombatHUD.h"


void APlayerNonCombatController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);

	GetControlledPlayer()->SetIsCombat(false);
}

void APlayerNonCombatController::ShortCut(EShortCutType _key)
{
	Super::ShortCut(_key);

	TObjectPtr<ANonCombatHUD> HUD = Cast<ANonCombatHUD>(GetHUD());
	if (nullptr == HUD)
		return;

	if(_key == EShortCutType::TAB)
		HUD->ShowMaintenanceUI();
	else if(_key == EShortCutType::INVENTORY)
		HUD->ShowInventoryUI();
}
