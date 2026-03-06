// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveLoadManager.generated.h"

class UARPGSaveGame;
class UItem;
enum class EItemType :uint8;

DECLARE_DELEGATE_OneParam(FOnSaveComplete, bool);
DECLARE_DELEGATE_OneParam(FOnLoadComplete, TObjectPtr<class USaveGame>);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API USaveLoadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	struct FPlayerDataParam 
	{
		FString PlayerName; 
		uint32 Gold;
		TWeakObjectPtr<class UInventory> Inventory;
	};

private:
	const int32 DEFAULT_SLOT_INDEX = 0;

public:
	void SavePlayerData(FPlayerDataParam& _param, FOnSaveComplete _callback);
	void LoadPlayerData(FOnLoadComplete _callback);
	bool DoesPlayerDataExist() const;
};
