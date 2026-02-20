// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuickSlot.generated.h"

class UItem;
class UConsumableItem;
class UInventory;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotChanged, uint8, TWeakObjectPtr<UConsumableItem>);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UQuickSlot : public UObject
{
	GENERATED_BODY()

public:
	UQuickSlot();

private:
	UPROPERTY()
	TWeakObjectPtr<UInventory> Inventory; // 인벤토리 약참조

	UPROPERTY()
	TArray<TWeakObjectPtr<UConsumableItem>> Container;

	bool IsValidSlot(uint8 _index) const;

public:
	FOnQuickSlotChanged OnQuickSlotChanged;

	void Init(TWeakObjectPtr<UInventory> _inventory, uint8 _size = 4);
	void Register(uint8 _index, TWeakObjectPtr<UItem> _consumableItem);
	TWeakObjectPtr<UConsumableItem> Unregister(uint8 _index);

	const TArray<TWeakObjectPtr<UConsumableItem>>& GetContainer() const { return Container; }
	TWeakObjectPtr<UConsumableItem> GetItem(uint8 _index) const { return Container[_index]; }

	void UseItem(uint8 _index, class IEffectable* _target);
};
