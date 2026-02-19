// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

class UItem;
class UItemConfig;
struct FItemData;
enum class EItemType : uint8;

struct FAddItemParam 
{
public:
	FName ID;
	uint16 Amount;
	uint8 OutIndex;
	FItemData* Data;
};

USTRUCT()
struct FItemArray 
{
	GENERATED_BODY()
public:
	TArray<TObjectPtr<UItem>> Array;
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
	TMap<EItemType, FItemArray> Container;

	bool TryFindEmpty(EItemType _type, uint8& _outIdx);

public:
	FOnInventoryChanged OnInventoryChanged;

	void Init(uint8 _size = 100);

	TObjectPtr<UItem> CreateItem(FAddItemParam& _param);
	bool TryAddItem(FAddItemParam& _param);
	bool TryAddItem(TObjectPtr<UItem> _item, uint8& _outIndex);
	bool TrySubItem(EItemType _type, uint8 _idx, uint16 _amount);
	bool TryFindItem(EItemType _type, const FName& _id, uint8& _outIdx, TFunction<bool(TObjectPtr<UItem>)> _predicate = nullptr) const;

	uint8 GetContainerSize() const;
	TObjectPtr<UItem> GetItem(EItemType _type, uint8 _idx) const { return Container[_type].Array[_idx]; }
	const TArray<TObjectPtr<UItem>>& GetContainer(EItemType _type) const { return Container[_type].Array; }
};
