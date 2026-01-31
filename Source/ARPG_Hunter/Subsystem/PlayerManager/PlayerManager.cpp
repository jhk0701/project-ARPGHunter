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
