// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/NonCombatGameMode.h"

#include "Core/Subsystem/SaveLoadManager.h"
#include "Controller/PlayerNonCombatController.h"
#include "UI/NonCombatHUD.h"


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
