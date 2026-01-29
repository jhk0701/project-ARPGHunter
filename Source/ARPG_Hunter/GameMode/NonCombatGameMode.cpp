// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NonCombatGameMode.h"
#include "Player/PlayerCharacter.h"
#include "Controller/PlayerCharacterController.h"

ANonCombatGameMode::ANonCombatGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerFinder(TEXT("/Game/02-BP/Player/BP_Player.BP_Player_C"));
	if (PlayerFinder.Succeeded())
		DefaultPawnClass = PlayerFinder.Class;

	PlayerControllerClass = APlayerCharacterController::StaticClass();
}
