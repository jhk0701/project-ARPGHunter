// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveGame/ARPGSaveGame.h"
#include "PlayerSaveGame.generated.h"

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
	uint16 Amount{0};
	UPROPERTY()
	uint8 InventoryIndex{0};
	UPROPERTY()
	int8 QuickSlotIndex{-1};
	UPROPERTY()
	int8 EquipmentIndex{-1};
	UPROPERTY()
	uint8 Grade{0};
};

USTRUCT()
struct FItemSaveDataArray
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<FItemSaveData> ItemArray;
};

USTRUCT()
struct FSkillNodeData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TMap<uint8, int8> Node;
};


/**
 *
 */
UCLASS()
class ARPG_HUNTER_API UPlayerSaveGame : public UARPGSaveGame
{
	GENERATED_BODY()

public:
	UPlayerSaveGame()
	{
		SlotName = TEXT("ARPG_PlayerData");
	}

	UPROPERTY()
	FString PlayerName;
	UPROPERTY()
	uint16 Level{ 0 };
	UPROPERTY()
	uint32 Exp{ 0 };
	UPROPERTY()
	uint32 Gold{ 0 };
	UPROPERTY()
	TMap<uint8, FItemSaveDataArray> InventoryDataMap;

	UPROPERTY()
	uint16 SkillPoint{ 0 };
	UPROPERTY()
	uint16 UsingSkillPoint{ 0 };
	UPROPERTY()
	TMap<uint8, FSkillNodeData> SkillTreeData;

	void SetInventoryData(TWeakObjectPtr<class UInventory> _inventory);
	void GetInventoryData(TWeakObjectPtr<class UInventory> _inventory, TFunctionRef<void(EItemType, TObjectPtr<class UItem>)> _slotedItemProcess);
	void SetSkillTreeData(TWeakObjectPtr<class USkillDevelop> _skillDevelop);
	void GetSkillTreeData(TWeakObjectPtr<class USkillDevelop> _skillDevelop);
};
