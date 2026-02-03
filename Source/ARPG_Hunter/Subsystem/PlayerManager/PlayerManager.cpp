// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/PlayerManager/PlayerManager.h"

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
	Stat[ECharacterStatType::HEALTH]					= 10;
	Stat[ECharacterStatType::STAMINA]					= 100;
	Stat[ECharacterStatType::SKILL]						= 100;
	Stat[ECharacterStatType::ATTACK]					= 20;
	Stat[ECharacterStatType::DEFENSE]					= 20;
	Stat[ECharacterStatType::CRITICAL_PERCENT]			= 10;
	Stat[ECharacterStatType::CRITICAL_DAMAGE_PERCENT]	= 100;

	// TODO : 플레이어 저장 데이터 적용하기
}

void UPlayerManager::AddGold(uint32 _amount)
{
	Gold.Value += _amount;
	Gold.OnValueChanged.Broadcast(Gold.Value);
}

void UPlayerManager::AddItem(const FName& _itemID, const uint8 _cnt)
{
	// TODO : 아이템 시스템 추가
}
