// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/ARPGGameMode.h"
#include "CombatGameMode.generated.h"

struct FStageData;
struct FSection;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ACombatGameMode : public AARPGGameMode
{
	GENERATED_BODY()

private:
	FStageData* StageData;
	TArray<bool> bSectionIsCleared;

public:
	ACombatGameMode();
	const FSection& GetSection(uint8 _idx) const;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

};
