// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

class UItem;
class UItemConfig;
class UDataManager;

struct FAddItemParam 
{
public:
	TObjectPtr<UDataManager> DataManager;
	const FName& ID;
	uint16 Amount;
	uint8& OutIdx;

	FAddItemParam(TObjectPtr<UDataManager> _dataManager, const FName& _id, uint16 _amount, uint8& _outIdx)
		: DataManager(_dataManager), ID(_id), Amount(_amount), OutIdx(_outIdx)
	{};
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, uint8, TObjectPtr<UItem>);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UInventory : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UItem>> Container;

	TObjectPtr<UItem> CreateItem(const FAddItemParam& _param);
	bool TryFindEmpty(uint8& _outIdx);

public:
	FOnInventoryChanged OnInventoryChanged;

	void Init(uint8 _size = 80);

	bool TryAddItem(FAddItemParam& _param);
	bool TrySubItem(uint8 _idx, uint16 _amount);
	bool TryFindItem(const FName& _id, uint8& _outIdx, TFunction<bool(TObjectPtr<UItem>)> _predicate = nullptr) const;

	TObjectPtr<UItem> GetItem(uint8 _idx) const { return Container[_idx]; }
};
