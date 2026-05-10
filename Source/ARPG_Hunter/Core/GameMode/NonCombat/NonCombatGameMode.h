// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/ARPGGameMode.h"
#include "NonCombatGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ANonCombatGameMode : public AARPGGameMode
{
	GENERATED_BODY()
	
public:
	ANonCombatGameMode();

protected:
	virtual void BeginPlay() override;
};
