// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/ARPGGameMode.h"
#include "CombatGameMode.generated.h"

enum class EMonsterType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ACombatGameMode : public AARPGGameMode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "StageSetting|Monster")
	TMap<EMonsterType, TSubclassOf<class AMonsterBase>> MonsterClass;

public:
	ACombatGameMode();

protected:
	virtual void PostInitializeComponents() override;

};
