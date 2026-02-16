// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Subsystem/PlayerManager.h"

#include "Core/Subsystem/DataManager.h"
#include "Define/Enum.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"


UPlayerManager::UPlayerManager()
{
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

	Stat[ECharacterStatType::HEALTH]					= 500;
	Stat[ECharacterStatType::STAMINA]					= 100;
	Stat[ECharacterStatType::SKILL]						= 100;
	Stat[ECharacterStatType::ATTACK]					= 20;
	Stat[ECharacterStatType::DEFENSE]					= 20;
	Stat[ECharacterStatType::CRITICAL_PERCENT]			= 10;
	Stat[ECharacterStatType::CRITICAL_DAMAGE_PERCENT]	= 100;

	// TODO : 플레이어 저장 데이터 적용하기
	Inventory->Init();
	Equipment->Init();
}

void UPlayerManager::Deinitialize()
{
	Super::Deinitialize();

	Inventory = nullptr;
	Equipment = nullptr;
}

void UPlayerManager::AddGold(uint32 _amount)
{
	Gold.Value += _amount;
	Gold.OnValueChanged.Broadcast(Gold.Value);
}

void UPlayerManager::AddItem(const FName& _itemID, int32 _amount)
{
	//아이템 추가
	uint8 Index;

	FAddItemParam Param(GetGameInstance()->GetSubsystem<UDataManager>(), _itemID, _amount, Index);
	bool bIsSuccess = Inventory->TryAddItem(Param);
}