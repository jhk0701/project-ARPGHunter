// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Subsystem/PlayerManager.h"

#include "Core/Subsystem/DataManager.h"
#include "Define/Enum.h"
#include "Data/PlayerConfig.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"
#include "Player/QuickSlot.h"
#include "Item/Item.h"

UPlayerManager::UPlayerManager()
{
	static ConstructorHelpers::FObjectFinder<UPlayerConfig> PlayerConfigFinder(TEXT("/Script/ARPG_Hunter.PlayerConfig'/Game/03-Data/PlayerDefaultConfig.PlayerDefaultConfig'"));
	if (PlayerConfigFinder.Succeeded())
		PlayerDefault = PlayerConfigFinder.Object;

	for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
	{
		ECharacterStatType type = static_cast<ECharacterStatType>(i);
		
		if (nullptr == Stat.Find(type))
			Stat.Add(type);
	}
}

void UPlayerManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Inventory = NewObject<UInventory>(this);
	Equipment = NewObject<UEquipment>(this);
	QuickSlot = NewObject<UQuickSlot>(this);

	Stat = PlayerDefault->InitStat;

	// TODO : 플레이어 저장 데이터 적용하기
	Inventory->Init();
	Equipment->Init();
	QuickSlot->Init();

	Equipment->OnStatValueChanged.AddUObject(this, &UPlayerManager::EquipmentStatChanged);
	QuickSlot->OnQuickSlotUsed.AddUObject(this, &UPlayerManager::QuickSlotItemUsed);

	ProvideBasicProperty();
}

const TMap<ECharacterStatType, uint32>& UPlayerManager::GetEquipmentStat() const
{
	return Equipment->GetEquipmentStat();
}

void UPlayerManager::AddGold(uint32 _amount)
{
	Gold.Value += _amount;
	Gold.OnValueChanged.Broadcast(Gold.Value);
}

bool UPlayerManager::TrySubGold(uint32 _amount)
{
	if(Gold.Value < _amount)
		return false;

	Gold.Value -= _amount;
	Gold.OnValueChanged.Broadcast(Gold.Value);

	return true;
}


uint8 UPlayerManager::AddItem(const FName& _itemID, int32 _amount)
{
	//아이템 추가
	FAddItemParam Param;
	Param.ID = _itemID;
	Param.Amount = _amount;
	Param.Data = GetGameInstance()->GetSubsystem<UDataManager>()->GetItemData(_itemID);

	Inventory->TryAddItem(Param);

	return Param.OutIndex;
}

void UPlayerManager::EquipmentStatChanged(const TMap<ECharacterStatType, uint32>& _equipmentStat)
{
	OnStatValueChanged.Broadcast(Stat, _equipmentStat);
}

void UPlayerManager::QuickSlotItemUsed(uint8 _quickSlotIdx, uint8 _inventoryIdx)
{
	Inventory->TrySubItem(EItemType::CONSUMABLE, _inventoryIdx, 1);

	// 사용 후 소모템을 모두 소진한 경우, 등록한 슬롯 비우기
	if (Inventory->GetItem(EItemType::CONSUMABLE, _inventoryIdx).IsValid() == false)
		QuickSlot->ClearSlot(_quickSlotIdx);
}

void UPlayerManager::ProvideBasicProperty()
{
	Gold.Value = 5000;

	AddItem(FName(TEXT("1001")), 50);
	AddItem(FName(TEXT("1002")), 50);
	AddItem(FName(TEXT("1003")), 50);
	AddItem(FName(TEXT("1004")), 50);
	
	uint8 Index = 0;
	Index = AddItem(FName(TEXT("3001")), 1);
	Equipment->Equip(EEquipmentType::HEAD, Inventory->GetItem(EItemType::ARMOR, Index));
	
	Index = AddItem(FName(TEXT("3002")), 1);
	Equipment->Equip(EEquipmentType::TOP, Inventory->GetItem(EItemType::ARMOR, Index));
	
	Index = AddItem(FName(TEXT("3003")), 1);
	Equipment->Equip(EEquipmentType::BOTTOM, Inventory->GetItem(EItemType::ARMOR, Index));
	
	Index = AddItem(FName(TEXT("4001")), 1);
	Equipment->Equip(EEquipmentType::WEAPON, Inventory->GetItem(EItemType::WEAPON, Index));

	Index = AddItem(FName(TEXT("2001")), 3);
	QuickSlot->Register(0, Inventory->GetItem(EItemType::CONSUMABLE, Index));

	AddItem(FName(TEXT("2002")), 3);
	AddItem(FName(TEXT("2003")), 3);
}

TObjectPtr<USkeletalMesh> UPlayerManager::GetDefaultMesh(EEquipmentType _type) const
{
	return PlayerDefault->MapDefalutMesh[_type];
}

TWeakObjectPtr<UConsumableItem> UPlayerManager::GetQuickSlotItem(uint8 _idx) const
{
	return QuickSlot->GetItem(_idx);
}

void UPlayerManager::UseQuickSlotItem(uint8 _index, IEffectable* _target)
{
	QuickSlot->UseItem(_index, _target);
}
