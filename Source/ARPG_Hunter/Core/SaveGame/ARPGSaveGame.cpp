// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SaveGame/ARPGSaveGame.h"

#include "Define/Enum.h"
#include "Player/Inventory.h"
#include "Item/Item.h"

FString UPlayerSaveData::GetSlotName()
{
	return TEXT("ARPG_PlayerData");
}

void UPlayerSaveData::SetInventoryData(TWeakObjectPtr<UInventory> _inventory)
{
	if (_inventory.IsValid() == false)
		return;

	for (uint8 i = 0; i < static_cast<uint8>(EItemType::END); ++i)
	{
		EItemType Type = static_cast<EItemType>(i);
		const TArray<TObjectPtr<UItem>>& Container = _inventory->GetContainer(Type);

		if (InventoryDataMap.Find(i) == nullptr)
		{
			FItemSaveDataArray& Array = InventoryDataMap.Add(i);
			Array.ItemArray.Reserve(Container.Num());
		}

		for (uint8 j = 0; j < Container.Num(); ++j)
		{
			TObjectPtr<UItem> Item = Container[j];
			if (Container[j] == nullptr)
				continue;

			FItemSaveData Save;

			Save.ID = Item->GetID();
			Save.Type = Item->GetType();
			Save.Amount = Item->GetAmount();
			Save.InventoryIndex = Item->GetInventoryIndex();
			
			if (Type == EItemType::CONSUMABLE)
			{
				TObjectPtr<UConsumableItem> Consumable = Cast<UConsumableItem>(Item);
				Save.QuickSlotIndex = Consumable->GetQuickSlotIndex();
			}
			else if (Type >= EItemType::EQUIPABLE)
			{
				TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(Item);
				Save.EquipmentIndex = Equipment->GetEquipmentIndex();
				Save.Grade = Equipment->GetGrade();
			}

			InventoryDataMap[i].ItemArray.Add(Save);
		}
	}
}
