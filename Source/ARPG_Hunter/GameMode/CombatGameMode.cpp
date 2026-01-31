// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CombatGameMode.h"

#include "Controller/PlayerCombatController.h"
#include "UI/PlayerHUD.h"

ACombatGameMode::ACombatGameMode()
{
	PlayerControllerClass = APlayerCombatController::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
}
