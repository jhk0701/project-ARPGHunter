// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/Subsystem.h"
#include "SaveLoadManager.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API USaveLoadManager : public USubsystem
{
	GENERATED_BODY()

public:
	USaveLoadManager();

private:


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
