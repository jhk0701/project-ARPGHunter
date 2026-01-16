// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/DungeonGameMode.h"
#include "Player/PlayerCharacter.h"
#include "Controller/PlayerCharacterController.h"

ADungeonGameMode::ADungeonGameMode()
{
	// TSubclassOf<ACharacter> PlayerCharacter = 
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerFinder(TEXT("/Game/02-BP/Player/BP_Player.BP_Player_C"));
	if (PlayerFinder.Succeeded())
		DefaultPawnClass = PlayerFinder.Class;

	PlayerControllerClass = APlayerCharacterController::StaticClass();
}
