// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Equipment.generated.h"

class UItem;
class UEquipmentItem;
class UEquipmentItemConfig;
enum class EEquipmentType : uint8;
enum class ECharacterStatType :uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentType, TObjectPtr<UEquipmentItem>);

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
	FOnEquipmentChanged OnEquipmentChanged;

	void Init();
	TObjectPtr<UEquipmentItem> Equip(EEquipmentType _type, TObjectPtr<UEquipmentItem> _equipment);
	TObjectPtr<UEquipmentItem> Unequip(EEquipmentType _type);

	const TObjectPtr<UEquipmentItem> GetEquipment(EEquipmentType _type) const { return Container[_type]; }
	const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& GetContainer() const { return Container; }
	const TMap<ECharacterStatType, uint32>& GetEquipmentStat() const { return EquipmentStat; }
};
