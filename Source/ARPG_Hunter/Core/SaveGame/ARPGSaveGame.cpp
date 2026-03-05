// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SaveGame/ARPGSaveGame.h"

#include "Define/Enum.h"
#include "Item/Item.h"

void UInventorySaveData::SetInventoryData(const TMap<EItemType, TArray<TObjectPtr<class UItem>>>& _container)
{
	for (uint8 i = 0; i < static_cast<uint8>(EItemType::END); ++i)
	{
		EItemType Type = static_cast<EItemType>(i);
		if (InventoryDataMap.Find(i) == nullptr)
		{
			FItemSaveDataArray& Array = InventoryDataMap.Add(i);
			Array.ItemArray.Reserve(_container[Type].Num());
		}

		for (uint8 j = 0; j < _container[Type].Num(); ++j)
		{
			TObjectPtr<UItem> Item = _container[Type][j];
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
