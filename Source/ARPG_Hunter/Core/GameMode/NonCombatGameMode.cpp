// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/NonCombatGameMode.h"
#include "Controller/PlayerCharacterController.h"

ANonCombatGameMode::ANonCombatGameMode()
{
	PlayerControllerClass = APlayerCharacterController::StaticClass();
}
