// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Equipment.generated.h"

class UEquipmentItem;
class UEquipmentItemConfig;
enum class EEquipmentType : uint8;
enum class ECharacterStatType :uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UEquipment : public UObject
{
	GENERATED_BODY()

public:
	UEquipment();

private:
	UPROPERTY()
	TMap<EEquipmentType, TObjectPtr<UEquipmentItem>> Container;
	TMap<ECharacterStatType, uint32> EquipmentStat;

public:
	void Init();
	TObjectPtr<UEquipmentItem> Equip(EEquipmentType _type, TObjectPtr<UEquipmentItem> _equipment);
	TObjectPtr<UEquipmentItem> Unequip(EEquipmentType _type);
};
