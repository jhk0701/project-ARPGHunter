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

using FEquipmentAliasMapStat = TMap<ECharacterStatType, uint32>;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EEquipmentType, TWeakObjectPtr<UEquipmentItem>);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentStatChanged, const FEquipmentAliasMapStat&);

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
	TWeakObjectPtr<UGameInstance> GI;

	UPROPERTY()
	TMap<EEquipmentType, TWeakObjectPtr<UEquipmentItem>> Container; // 아이템 약참조. 실소유권은 인벤토리에게 있음
	TMap<ECharacterStatType, uint32> EquipmentStat;

public:
	FOnEquipmentChanged OnEquipmentChanged;
	FOnEquipmentStatChanged OnStatValueChanged;

	void Init(TWeakObjectPtr<UGameInstance> _instance);
	void Equip(EEquipmentType _type, TWeakObjectPtr<UItem> _equipment);
	TWeakObjectPtr<UEquipmentItem> Unequip(EEquipmentType _type);

	const TWeakObjectPtr<UEquipmentItem> GetEquipment(EEquipmentType _type) const { return Container[_type]; }
	const TMap<EEquipmentType, TWeakObjectPtr<UEquipmentItem>>& GetContainer() const { return Container; }
	const TMap<ECharacterStatType, uint32>& GetEquipmentStat() const { return EquipmentStat; }

	void LoadEquipment(TObjectPtr<UItem> _item);
};
