// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/CombatGameMode.h"
#include "TestGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ATestGameMode : public ACombatGameMode
{
	GENERATED_BODY()

protected:
	void PostInitializeComponents() override;
	
};
