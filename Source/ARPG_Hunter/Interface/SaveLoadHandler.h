// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveLoadHandler.generated.h"


class UARPGSaveGame;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveLoadHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_HUNTER_API ISaveLoadHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void WriteSaveData(UARPGSaveGame* _savegame) = 0;
	virtual void ReadSaveData(UARPGSaveGame* _savegame) = 0;
};
