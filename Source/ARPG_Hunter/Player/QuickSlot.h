// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuickSlot.generated.h"

class UConsumableItem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotChanged, uint8, TObjectPtr<UConsumableItem>);

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
	TArray<TObjectPtr<UConsumableItem>> Container;

public:
	FOnQuickSlotChanged OnQuickSlotChanged;

	void Init(uint8 _size = 4);
	TObjectPtr<UConsumableItem> Register(uint8 _index, TObjectPtr<UConsumableItem> _consumableItem);
	TObjectPtr<UConsumableItem> Unregister(uint8 _index);

	const TArray<TObjectPtr<UConsumableItem>>& GetContainer() const { return Container; }

	void UseItem(uint8 _index);
};
