// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ARPGSaveGame.generated.h"


enum class EItemType :uint8;

USTRUCT()
struct FItemSaveData 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName ID;
	UPROPERTY()
	EItemType Type;
	UPROPERTY()
	uint16 Amount;
	UPROPERTY()
	uint8 InventoryIndex;
	UPROPERTY()
	int8 QuickSlotIndex;
	UPROPERTY()
	int8 EquipmentIndex;
	UPROPERTY()
	uint8 Grade;
};

USTRUCT()
struct FItemSaveDataArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FItemSaveData> ItemArray;
};

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UARPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	uint32 UserIndex{ 0 };
	UPROPERTY(VisibleAnywhere)
	FString Version;
	UPROPERTY(VisibleAnywhere)
	FString SaveSlotName;

	void InitSaveGame(uint32 _userIdx, const FString& _ver, const FString& _slotName) 
	{
		UserIndex = _userIdx;
		Version = _ver;
		SaveSlotName = _slotName;
	};
};

UCLASS()
class ARPG_HUNTER_API UPlayerSaveData : public UARPGSaveGame
{
public:
	UPROPERTY(VisibleAnywhere)
	FString PlayerName;
	UPROPERTY(VisibleAnywhere)
	uint32 Gold{ 0 };
};

UCLASS()
class ARPG_HUNTER_API UInventorySaveData : public UARPGSaveGame
{
public:
	TMap<uint8, FItemSaveDataArray> InventoryDataMap;
	
	void SetInventoryData(const TMap<EItemType, TArray<TObjectPtr<class UItem>>>& _container);
};

/*

*/