// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameMode.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API AARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AARPGGameMode();
	
	void OpenLevel(const FName& _levelName);
	void BackToTown();
};
