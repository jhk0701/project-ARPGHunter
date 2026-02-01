// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Define/Enum.h"
#include "PlayerManager.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UPlayerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	TMap<ECharacterStatType, uint32> Stat;

public:
	UPlayerManager();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const TMap<ECharacterStatType, uint32>& GetPlayerStat() const { return Stat; }
};
