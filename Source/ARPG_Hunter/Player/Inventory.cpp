// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Inventory.h"
#include "Kismet/GameplayStatics.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager.h"
#include "Data/ItemData.h"
#include "Item/Item.h"

void UInventory::Init(uint8 _size)
{
	Container.SetNum(_size);
}

TObjectPtr<UItem> UInventory::CreateInstance(TObjectPtr<UObject> _worldContext, const FName& _id, uint16 _amount)
{
	TObjectPtr<UDataManager> DataManager = UGameplayStatics::GetGameInstance(_worldContext)->GetSubsystem<UDataManager>();
	FItemData* ItemData = DataManager->GetItemData(_id);
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
		Instance->Init(_id, _amount, ItemData->Item);

	return Instance;
}


bool UInventory::TryAddItem(TObjectPtr<UObject> _worldContext, const FName& _id, uint16 _amount, uint8& _outIdx)
{
	uint8 Index = 0;
	if (TryFindItem(_id, Index, [](TObjectPtr<UItem> _existItem) { return _existItem->IsFull() == false; }))
	{
		// 기존 아이템 추가 획득
		uint16 RemainAmount = 0;
		if (Container[Index]->TryAddAmount(_amount, RemainAmount)) 
		{
			// 남김없이 다 추가된 경우
			_outIdx = Index;
			return true;
		}

		_amount = RemainAmount; // 획득 후, 해당 슬롯이 다 차서 남은 갯수 -> 신규 획득 처리
	}

	// 신규 획득
	// 남은 공간 확인
	if (TryFindEmpty(Index) == false)
		return false; // 여유 공간이 없는 상황

	// 신규 아이템 인스턴스 추가
	Container[Index] = CreateInstance(_worldContext, _id, _amount);
	_outIdx = Index;

	return true;
}

bool UInventory::TrySubItem(uint8 _idx)
{
	return false;
}

bool UInventory::TryFindItem(const FName& _id, uint8& _outIdx, TFunction<bool(TObjectPtr<UItem>)> _predicate)
{
	for (uint8 i = 0; i < Container.Num(); ++i)
	{
		if (Container[i]->GetID() != _id)
			continue;

		if((_predicate && _predicate(Container[i])) || nullptr == _predicate)
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