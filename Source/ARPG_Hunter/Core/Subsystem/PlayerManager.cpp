// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Subsystem/PlayerManager.h"
#include "Core/Subsystem/DataManager.h"
#include "Define/Enum.h"
#include "Data/PlayerConfig.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"
#include "Player/QuickSlot.h"
#include "Item/Item.h"

#include "Core/Subsystem/SaveLoadManager.h"
#include "SaveGame/PlayerSaveGame.h"

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

	Stat = PlayerDefault->InitStat;
	Inventory = NewObject<UInventory>(this);
	Equipment = NewObject<UEquipment>(this);
	QuickSlot = NewObject<UQuickSlot>(this);

	FGetItemDataFunc GetItemDataFunc;
	GetItemDataFunc.BindLambda([this](const FName& _id) { return GetGameInstance()->GetSubsystem<UDataManager>()->GetItemData(_id); });
	Inventory->Init(GetItemDataFunc);
	Equipment->Init(GetGameInstance());
	QuickSlot->Init();

	Equipment->OnStatValueChanged.AddUObject(this, &UPlayerManager::EquipmentStatChanged);
	QuickSlot->OnQuickSlotUsed.AddUObject(this, &UPlayerManager::QuickSlotItemUsed);
}

void UPlayerManager::PostInit()
{
	TObjectPtr<USaveLoadManager> SaveLoad = GetGameInstance()->GetSubsystem<USaveLoadManager>();
	SaveLoad->RegisterHandler<UPlayerSaveGame>(this);
}

TWeakObjectPtr<UInventory> UPlayerManager::GetInventory() const { return Inventory; }
TWeakObjectPtr<UEquipment> UPlayerManager::GetEquipment() const { return Equipment; }
TWeakObjectPtr<UQuickSlot> UPlayerManager::GetQuickSlot() const { return QuickSlot; }
const TMap<ECharacterStatType, uint32>& UPlayerManager::GetEquipmentStat() const { return Equipment->GetEquipmentStat(); }

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
	UInventory::FCreateItemParam Param(_itemID, _amount);
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


void UPlayerManager::CreateNewPlayer(const FString& _playerName)
{
	PlayerName = _playerName;
	ProvideBasicProperty();
}

void UPlayerManager::ProvideBasicProperty()
{
	Gold.Value = 1000;
	/*
	AddItem(FName(TEXT("1001")), 10);
	AddItem(FName(TEXT("1002")), 10);
	AddItem(FName(TEXT("1003")), 5);
	AddItem(FName(TEXT("1004")), 5);
	*/
	uint8 Index = 0;
	Index = AddItem(FName(TEXT("3001")), 1);
	Equipment->Equip(EEquipmentType::HEAD, Inventory->GetItem(EItemType::ARMOR, Index));
	
	Index = AddItem(FName(TEXT("3002")), 1);
	Equipment->Equip(EEquipmentType::TOP, Inventory->GetItem(EItemType::ARMOR, Index));
	
	Index = AddItem(FName(TEXT("3003")), 1);
	Equipment->Equip(EEquipmentType::BOTTOM, Inventory->GetItem(EItemType::ARMOR, Index));
	
	Index = AddItem(FName(TEXT("4001")), 1);
	TWeakObjectPtr<UItem> Item = Inventory->GetItem(EItemType::WEAPON, Index);
	// TObjectPtr<UEquipmentItem> Weapon = Cast<UEquipmentItem>(Item);
	// Weapon->SetGrade(3);
	Equipment->Equip(EEquipmentType::WEAPON, Item);

	Index = AddItem(FName(TEXT("2001")), 5);
	QuickSlot->Register(0, Inventory->GetItem(EItemType::CONSUMABLE, Index));

	// AddItem(FName(TEXT("2002")), 10);
	// AddItem(FName(TEXT("2003")), 10);
	// AddItem(FName(TEXT("4002")), 1);
}

TObjectPtr<USkeletalMesh> UPlayerManager::GetDefaultMesh(EEquipmentType _type) const { return PlayerDefault->MapDefalutMesh[_type]; }
TWeakObjectPtr<UConsumableItem> UPlayerManager::GetQuickSlotItem(uint8 _idx) const { return QuickSlot->GetItem(_idx); }

void UPlayerManager::UseQuickSlotItem(uint8 _index, IEffectable* _target)
{
	QuickSlot->UseItem(_index, _target);
}


void UPlayerManager::WriteSaveData(UARPGSaveGame* _savegame)
{
	UPlayerSaveGame* PlayerSave = Cast<UPlayerSaveGame>(_savegame);
	PlayerSave->PlayerName = PlayerName;
	PlayerSave->Gold = Gold.Value;
	PlayerSave->SetInventoryData(GetInventory());
}

void UPlayerManager::ReadSaveData(UARPGSaveGame* _savegame)
{
	TObjectPtr<UPlayerSaveGame> PlayerSaveData = Cast<UPlayerSaveGame>(_savegame);
	PlayerName = PlayerSaveData->PlayerName;
	Gold.Value = PlayerSaveData->Gold;

	PlayerSaveData->GetInventoryData(GetInventory(), 
		[this](EItemType _type, TObjectPtr<UItem> _item) 
		{
			if (_type == EItemType::CONSUMABLE) 
				QuickSlot->LoadQuickSlot(_item);
			else if (_type >= EItemType::EQUIPABLE) 
				Equipment->LoadEquipment(_item);
		}
	);
}