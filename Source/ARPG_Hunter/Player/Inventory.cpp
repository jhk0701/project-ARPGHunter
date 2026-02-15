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
		Instance = NewObject<UItem>();
		break;
	case EItemType::WEAPON:
		Instance = NewObject<UEquipableItem>();
		break;
	case EItemType::ARMOR:
		Instance = NewObject<UEquipableItem>();
		break;
	}

	if (Instance)
		Instance->Init(_amount, ItemData->Item);

	return Instance;
}

bool UInventory::TryAddItem(TObjectPtr<UObject> _worldContext, const FName& _id, uint16 _amount, uint8& _outIdx)
{
	// ItemData->Item


	return false;
}

bool UInventory::TrySubItem(uint8 _idx)
{
	return false;
}

bool UInventory::TryFindItem(const FName& _id, uint8& _outIdx)
{
	
	return false;
}
