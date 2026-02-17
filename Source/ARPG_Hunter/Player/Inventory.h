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
	FName ID;
	uint16 Amount;
	uint8 OutIndex;

	FAddItemParam(TObjectPtr<UDataManager> _dataManager) : DataManager(_dataManager)
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

	bool TryFindEmpty(uint8& _outIdx);

public:
	FOnInventoryChanged OnInventoryChanged;

	void Init(uint8 _size = 80);

	TObjectPtr<UItem> CreateItem(const FAddItemParam& _param);
	bool TryAddItem(FAddItemParam& _param);
	bool TrySubItem(uint8 _idx, uint16 _amount);
	bool TryFindItem(const FName& _id, uint8& _outIdx, TFunction<bool(TObjectPtr<UItem>)> _predicate = nullptr) const;

	TObjectPtr<UItem> GetItem(uint8 _idx) const { return Container[_idx]; }
	const TArray<TObjectPtr<UItem>>& GetContainer() const { return Container; }
};
