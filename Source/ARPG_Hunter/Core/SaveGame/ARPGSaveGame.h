// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ARPGSaveGame.generated.h"

USTRUCT()
struct FItemSaveData 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FName ID;
	UPROPERTY()
	uint8 Type;
	UPROPERTY()
	uint16 Amount;
	UPROPERTY()
	uint8 InventoryIndex;
};

USTRUCT()
struct FConsumableItemSaveData : public FItemSaveData 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 QuickSlotIndex;
};

USTRUCT()
struct FEquipmentItemSaveData : public FItemSaveData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 EquipmentIndex;
	UPROPERTY()
	uint8 Grade;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UARPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UARPGSaveGame();

	UPROPERTY(VisibleAnywhere)
	FString PlayerName;
	UPROPERTY(VisibleAnywhere)
	FString SaveSlotName;
	UPROPERTY(VisibleAnywhere)
	uint32 UserIndex{0};

	UPROPERTY(VisibleAnywhere)
	uint32 Gold{0};

	// 인벤토리 목록
};
