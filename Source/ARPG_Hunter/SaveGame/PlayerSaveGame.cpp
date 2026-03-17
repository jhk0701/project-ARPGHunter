// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/PlayerSaveGame.h"

#include "Define/Enum.h"
#include "Player/Inventory.h"
#include "Player/SkillDevelop.h"
#include "Data/ItemData.h"
#include "Item/Item.h"

void UPlayerSaveGame::SetInventoryData(TWeakObjectPtr<UInventory> _inventory)
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

void UPlayerSaveGame::GetInventoryData(TWeakObjectPtr<UInventory> _inventory, TFunctionRef<void(EItemType, TObjectPtr<class UItem>)> _slotedItemProcess)
{
	if (_inventory.IsValid() == false)
		return;

	for (const TPair<uint8, FItemSaveDataArray>& Pair : InventoryDataMap)
	{
		EItemType Type = static_cast<EItemType>(Pair.Key);
		for (const FItemSaveData& ItemSaveData : Pair.Value.ItemArray)
		{
			UInventory::FCreateItemParam Param(ItemSaveData.ID, ItemSaveData.Amount);
			TObjectPtr<UItem> ItemInst = _inventory->CreateItem(Param);
			ItemInst->SetInventoryIndex(ItemSaveData.InventoryIndex);

			if (Type == EItemType::CONSUMABLE) 
			{
				TObjectPtr<UConsumableItem> Consumable = Cast<UConsumableItem>(ItemInst);
				Consumable->SetQuickSlotIndex(ItemSaveData.QuickSlotIndex);

				if (ItemSaveData.QuickSlotIndex >= 0)
					_slotedItemProcess(Type, ItemInst);
			}
			else if (Type >= EItemType::EQUIPABLE) 
			{
				TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(ItemInst);
				Equipment->SetEquipmentIndex(ItemSaveData.EquipmentIndex);
				Equipment->SetGrade(ItemSaveData.Grade);

				if (ItemSaveData.EquipmentIndex >= 0)
					_slotedItemProcess(Type, ItemInst);
			}

			_inventory->SetItem(Type, ItemInst->GetInventoryIndex(), ItemInst);
		}
	}
}

void UPlayerSaveGame::SetSkillTreeData(TWeakObjectPtr<class USkillDevelop> _skillDevelop)
{
	if (false == _skillDevelop.IsValid())
		return;

	const TMap<uint8, TMap<uint8, int8>>& SelectMap = _skillDevelop->GetSkillSelect();
	for (const TPair<uint8, TMap<uint8, int8>>& Trees : SelectMap)
	{
		FSkillNodeData& NodeData = SkillTreeData.Add(Trees.Key);
		NodeData.Node = Trees.Value;
	}
}

void UPlayerSaveGame::GetSkillTreeData(TWeakObjectPtr<class USkillDevelop> _skillDevelop)
{
	if (false == _skillDevelop.IsValid())
		return;

	for (const TPair<uint8, FSkillNodeData>& TreeData : SkillTreeData)
	{
		for (const TPair<uint8, int8>& NodeData : TreeData.Value.Node)
			_skillDevelop->AddSkill(TreeData.Key, NodeData.Key, NodeData.Value);
	}
}
