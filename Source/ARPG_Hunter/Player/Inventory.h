// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

class UItem;
class UItemConfig;
struct FItemData;
enum class EItemType : uint8;

USTRUCT()
struct FItemArray 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<TObjectPtr<UItem>> Array;
};

DECLARE_DELEGATE_RetVal_OneParam(FItemData*, FGetItemDataFunc, const FName&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, uint8, TWeakObjectPtr<UItem>);

constexpr uint8 DEFAULT_INVENTORY_SIZE = 100;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UInventory : public UObject
{
	GENERATED_BODY()
public:
	// 신규 아이템 생성시 사용
	struct FCreateItemParam
	{
	public:
		FName ID;
		uint16 Amount;
		uint8 OutIndex{0};
		FItemData* Data{nullptr};

		FCreateItemParam(const FName& _id, uint16 _amount) : ID(_id), Amount(_amount) {};
	};

private:
	FGetItemDataFunc GetItemDataFunc;

	UPROPERTY()
	TMap<EItemType, FItemArray> Container;

	bool TryFindEmpty(EItemType _type, uint8& _outIdx);
public:
	FOnInventoryChanged OnInventoryChanged;

	void Init(const FGetItemDataFunc&& _getItemDataFunc, uint8 _size = DEFAULT_INVENTORY_SIZE);
	bool IsValid() const { return GetItemDataFunc.IsBound(); };

	void SetItem(EItemType _type, uint8 _idx, TObjectPtr<UItem> _itemInst);
	TObjectPtr<UItem> CreateItem(FCreateItemParam& _param);
	bool TryAddItem(FCreateItemParam& _param);
	bool TryAddItem(TObjectPtr<UItem> _item, uint8& _outIndex);
	bool TrySubItem(EItemType _type, uint8 _idx, uint16 _amount);
	bool TryFindItem(EItemType _type, const FName& _id, uint8& _outIdx, TFunction<bool(TObjectPtr<UItem>)> _predicate = nullptr) const;
	void SearchItems(EItemType _type, TSet<uint8>& _outIdxSet, TFunction<bool(TObjectPtr<UItem>)> _predicate = nullptr) const;

	uint8 GetContainerSize() const;
	TWeakObjectPtr<UItem> GetItem(EItemType _type, uint8 _idx) const;
	const TArray<TObjectPtr<UItem>>& GetContainer(EItemType _type) const { return Container[_type].Array; }
};
