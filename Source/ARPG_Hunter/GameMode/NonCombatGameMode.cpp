// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NonCombatGameMode.h"
#include "Controller/PlayerCharacterController.h"

ANonCombatGameMode::ANonCombatGameMode()
{
	PlayerControllerClass = APlayerCharacterController::StaticClass();
}
