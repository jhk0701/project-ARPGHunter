// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inventory.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager.h"
#include "Data/ItemData.h"
#include "Item/Item.h"


void UInventory::Init(uint8 _size)
{
	Container.SetNum(_size);
}

TObjectPtr<UItem> UInventory::CreateItem(const FAddItemParam& _param)
{
	FItemData* ItemData = _param.DataManager->GetItemData(_param.ID);
	if (nullptr == ItemData)
		return nullptr;

	TObjectPtr<UItem> Instance = nullptr;
	switch (ItemData->Type)
	{
	case EItemType::ITEM:
		Instance = NewObject<UItem>();
		break;
	case EItemType::CONSUMABLE:
		Instance = NewObject<UConsumableItem>();
		break;
	case EItemType::WEAPON: __fallthrough;
	case EItemType::ARMOR:
		Instance = NewObject<UEquipmentItem>();
		break;
	}

	if (Instance)
		Instance->Init(_param.ID, _param.Amount, ItemData->Item);

	return Instance;
}

bool UInventory::TryAddItem(FAddItemParam& _param)
{
	uint8 Index = 0;
	
	if (TryFindItem(_param.ID, Index, [](TObjectPtr<UItem> _existItem) { return _existItem->IsFull() == false; }))
	{
		// 기존 아이템 추가 획득
		uint16 RemainAmount = 0;
		if (Container[Index]->TryAddAmount(_param.Amount, RemainAmount))
		{
			// 남김없이 다 추가된 경우
			_param.OutIdx = Index;
			OnInventoryChanged.Broadcast(Index, Container[Index]);
			return true;
		}

		_param.Amount = RemainAmount; // 획득 후, 해당 슬롯이 다 차서 남은 갯수 -> 신규 획득 처리
		OnInventoryChanged.Broadcast(Index, Container[Index]);
	}

	// 신규 획득
	// 남은 공간 확인
	if (TryFindEmpty(Index) == false)
		return false; // 여유 공간이 없는 상황

	// 신규 아이템 인스턴스 추가
	Container[Index] = CreateItem(_param);
	_param.OutIdx = Index;

	OnInventoryChanged.Broadcast(Index, Container[Index]);

	return true;
}

bool UInventory::TrySubItem(uint8 _idx, uint16 _amount)
{
	if (nullptr == Container[_idx])
		return false;

	bool bIsSuccess = Container[_idx]->TrySubAmount(_amount);
	if (bIsSuccess) 
	{
		if (Container[_idx]->GetAmount() == 0)
			Container[_idx] == nullptr;

		OnInventoryChanged.Broadcast(_idx, Container[_idx]); // 이 시점에서 nullptr일 것
	}

	return bIsSuccess;
}

bool UInventory::TryFindItem(const FName& _id, uint8& _outIdx, TFunction<bool(TObjectPtr<UItem>)> _predicate) const
{
	for (uint8 i = 0; i < Container.Num(); ++i)
	{
		if (nullptr == Container[i] || Container[i]->GetID() != _id)
			continue;

		if((_predicate && _predicate(Container[i])) || _predicate == nullptr)
		{
			_outIdx = i;
			return true;
		}
	}
	
	return false;
}

bool UInventory::TryFindEmpty(uint8& _outIdx)
{
	for (uint8 i = 0; i < Container.Num(); ++i)
	{
		if (nullptr == Container[i])
		{
			_outIdx = i;
			return true;
		}
	}

	return false;
}