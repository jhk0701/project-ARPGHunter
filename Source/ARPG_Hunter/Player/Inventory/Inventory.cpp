// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inventory/Inventory.h"

#include "Define/Enum.h"
#include "Data/ItemData.h"
#include "Item/Item.h"


void UInventory::Init(const FGetItemDataFunc&& _getItemDataFunc, uint8 _size)
{
	GetItemDataFunc = _getItemDataFunc;

	for (uint8 i = 0; i < static_cast<uint8>(EItemType::END); ++i)
	{
		EItemType Type = static_cast<EItemType>(i);
		FItemArray& ItemArr = Container.Add(Type);
		ItemArr.Array.SetNum(_size);
	}
}

TObjectPtr<UItem> UInventory::CreateItem(FCreateItemParam& _param)
{
	if (IsValid() == false)
		return nullptr;

	if (_param.Data == nullptr)
		_param.Data = GetItemDataFunc.Execute(_param.ID);

	TObjectPtr<UItem> Instance = nullptr;
	switch (_param.Data->Type)
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
		Instance->Init(_param.ID, _param.Data->Type, _param.Amount, _param.Data->Item);

	return Instance;
}

bool UInventory::TryAddItem(FCreateItemParam& _param)
{
	if (IsValid() == false)
		return false;

	uint8 Index = 0;
	_param.Data = GetItemDataFunc.Execute(_param.ID);

	EItemType Type = _param.Data->Type;
	
	if (TryFindItem(Type, _param.ID, Index, [](TObjectPtr<UItem> _existItem) { return _existItem->IsFull() == false; }))
	{
		// 기존 아이템 추가 획득
		uint16 RemainAmount = 0;
		if (Container[Type].Array[Index]->TryAddAmount(_param.Amount, RemainAmount))
		{
			// 남김없이 다 추가된 경우
			_param.OutIndex = Index;
			OnInventoryChanged.Broadcast(Index, Container[Type].Array[Index]);
			return true;
		}

		_param.Amount = RemainAmount; // 획득 후, 해당 슬롯이 다 차서 남은 갯수 -> 신규 획득 처리
		OnInventoryChanged.Broadcast(Index, Container[Type].Array[Index]);
	}

	// 신규 획득
	// 남은 공간 확인
	if (TryFindEmpty(Type, Index) == false)
		return false; // 여유 공간이 없는 상황

	// 신규 아이템 인스턴스 추가
	Container[Type].Array[Index] = CreateItem(_param);
	Container[Type].Array[Index]->SetInventoryIndex(Index);
	_param.OutIndex = Index;

	OnInventoryChanged.Broadcast(Index, Container[Type].Array[Index]);

	return true;
}

bool UInventory::TryAddItem(TObjectPtr<UItem> _item, uint8& _outIndex)
{
	EItemType Type = _item->GetType();

	// 기존 아이템 확인
	if (TryFindItem(Type, _item->GetID(), _outIndex, [](TObjectPtr<UItem> _existItem) { return _existItem->IsFull() == false; }))
	{
		// 기존 아이템 추가 획득
		uint16 RemainAmount = 0;
		if (Container[Type].Array[_outIndex]->TryAddAmount(_item->GetAmount(), RemainAmount))
		{
			// 남김없이 다 추가된 경우
			OnInventoryChanged.Broadcast(_outIndex, Container[Type].Array[_outIndex]);
			return true;
		}

		_item->SetAmount(RemainAmount); // 획득 후, 해당 슬롯이 다 차서 남은 갯수 -> 신규 획득 처리
		OnInventoryChanged.Broadcast(_outIndex, Container[Type].Array[_outIndex]);
	}

	// 신규 획득
	// 남은 공간 확인
	if (TryFindEmpty(Type, _outIndex) == false)
		return false; // 여유 공간이 없는 상황

	// 신규 아이템 인스턴스 추가
	Container[Type].Array[_outIndex] = _item;
	Container[Type].Array[_outIndex]->SetInventoryIndex(_outIndex);
	OnInventoryChanged.Broadcast(_outIndex, Container[Type].Array[_outIndex]);

	return true;
}

bool UInventory::TrySubItem(EItemType _type, uint8 _idx, uint16 _amount)
{
	if (nullptr == Container[_type].Array[_idx])
		return false;

	bool bIsSuccess = Container[_type].Array[_idx]->TrySubAmount(_amount);
	if (bIsSuccess) 
	{
		if (Container[_type].Array[_idx]->GetAmount() == 0)
			Container[_type].Array[_idx] = nullptr;

		OnInventoryChanged.Broadcast(_idx, Container[_type].Array[_idx]); // 이 시점에서 nullptr일 것
	}

	return bIsSuccess;
}

bool UInventory::TryFindItem(EItemType _type, const FName& _id, uint8& _outIdx, TFunction<bool(TObjectPtr<UItem>)> _predicate) const
{
	for (uint8 i = 0; i < Container.Num(); ++i)
	{
		if (nullptr == Container[_type].Array[i] || Container[_type].Array[i]->GetID() != _id)
			continue;

		if((_predicate && _predicate(Container[_type].Array[i])) || _predicate == nullptr)
		{
			_outIdx = i;
			return true;
		}
	}
	
	return false;
}

void UInventory::SearchItems(EItemType _type, TSet<uint8>& _outIdxSet, TFunction<bool(TObjectPtr<UItem>)> _predicate) const
{
	for (uint8 i = 0; i < Container.Num(); ++i)
	{
		if (nullptr == Container[_type].Array[i] || 
			(_predicate && _predicate(Container[_type].Array[i]) == false))
			continue;

		_outIdxSet.Add(i);
	}
}

bool UInventory::TryFindEmpty(EItemType _type, uint8& _outIdx)
{
	for (uint8 i = 0; i < Container[_type].Array.Num(); ++i)
	{
		if (nullptr == Container[_type].Array[i])
		{
			_outIdx = i;
			return true;
		}
	}

	return false;
}

uint8 UInventory::GetContainerSize() const
{
	return Container.begin()->Value.Array.Num();
}

void UInventory::SetItem(EItemType _type, uint8 _idx, TObjectPtr<UItem> _itemInst)
{
	Container[_type].Array[_idx] = _itemInst;
}

TWeakObjectPtr<UItem> UInventory::GetItem(EItemType _type, uint8 _idx) const
{
	return Container[_type].Array[_idx];
}
