// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ARPGGameMode.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AARPGGameMode::AARPGGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerFinder(TEXT("/Game/02-BP/Player/BP_Player.BP_Player_C"));
	if (PlayerFinder.Succeeded())
		DefaultPawnClass = PlayerFinder.Class;
}

void AARPGGameMode::OpenLevel(FName _levelName)
{
	UGameplayStatics::OpenLevel(GetWorld(), _levelName);
}

void AARPGGameMode::BackToTown()
{
	OpenLevel(FName(TEXT("MainTown")));
}
