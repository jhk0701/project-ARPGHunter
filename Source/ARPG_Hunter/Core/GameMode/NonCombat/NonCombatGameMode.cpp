// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/NonCombat/NonCombatGameMode.h"

#include "Core/Subsystem/SaveLoadManager/SaveLoadManager.h"
#include "Controller/Player/PlayerNonCombatController.h"
#include "UI/HUD/NonCombat/NonCombatHUD.h"


ANonCombatGameMode::ANonCombatGameMode()
{
	PlayerControllerClass = APlayerNonCombatController::StaticClass();
	HUDClass = ANonCombatHUD::StaticClass();
}

void ANonCombatGameMode::BeginPlay()
{
	Super::BeginPlay();
	SaveGame();
}
