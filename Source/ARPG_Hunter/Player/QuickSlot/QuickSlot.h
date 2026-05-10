// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuickSlot.generated.h"

class UItem;
class UConsumableItem;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotUsed, uint8, uint8);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuickSlotChanged, uint8, TWeakObjectPtr<UConsumableItem>);

constexpr uint8 DEFAULT_QUICKSLOT_SIZE = 4;

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
	TArray<TWeakObjectPtr<UConsumableItem>> Container; // 아이템 약참조. 실소유권은 인벤토리에게 있음

	bool IsValidSlot(uint8 _index) const;

public:
	FOnQuickSlotUsed OnQuickSlotUsed;
	FOnQuickSlotUsed OnPostQuickSlotUsed;
	FOnQuickSlotChanged OnQuickSlotChanged;

	void Init(uint8 _size = DEFAULT_QUICKSLOT_SIZE);
	void Register(uint8 _index, TWeakObjectPtr<UItem> _consumableItem);
	TWeakObjectPtr<UConsumableItem> Unregister(uint8 _index);
	// 소모템을 모두 소진 시, 등록한 슬롯을 비우기 위한 용도
	void ClearSlot(uint8 _index);

	const TArray<TWeakObjectPtr<UConsumableItem>>& GetContainer() const { return Container; }
	TWeakObjectPtr<UConsumableItem> GetItem(uint8 _index) const { return Container[_index]; }

	void UseItem(uint8 _index, class IEffectable* _target);
	
	void LoadQuickSlot(TObjectPtr<UItem> _item);
};
