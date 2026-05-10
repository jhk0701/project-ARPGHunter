// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interface/SaveLoadHandler.h"
#include "SaveLoadManager.generated.h"

class UARPGSaveGame;

DECLARE_DELEGATE(FOnSaveLoadComplete);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API USaveLoadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	USaveLoadManager();

private:
	const int32 DEFAULT_SLOT_INDEX = 0;
	TMap<TSubclassOf<UARPGSaveGame>, ISaveLoadHandler*> MapHandler;

	UARPGSaveGame* CreateGetSaveGameObject(UClass* _class);
	void AsyncSaveGame(UARPGSaveGame* _savegame, FOnSaveLoadComplete& _callback);
	void AsyncLoadGame(UClass* Key, const FString& _slotName, int32 _slotIndex, FOnSaveLoadComplete& _callback);
	bool DoesSaveGameExist(const FString& _slotName, int32 _slotIndex);

public:
	template<typename ARPGSaveGameType>
	void RegisterHandler(ISaveLoadHandler* _handler);
	
	template<typename ARPGSaveGameType>
	void SaveGame(ISaveLoadHandler* _handler, FOnSaveLoadComplete& _callback);
	
	void SaveAll(FOnSaveLoadComplete& _callback);

	template<typename ARPGSaveGameType>
	void LoadGame(FOnSaveLoadComplete& _callback);
	
	template<typename ARPGSaveGameType>
	bool DoesDataExist();
};

template<typename ARPGSaveGameType>
inline void USaveLoadManager::RegisterHandler(ISaveLoadHandler* _handler)
{
	UClass* Key = ARPGSaveGameType::StaticClass();
	if (MapHandler.Find(Key) != nullptr)
		MapHandler[Key] = _handler;
	else
		MapHandler.Add(Key, _handler); 
}

template<typename ARPGSaveGameType>
inline void USaveLoadManager::SaveGame(ISaveLoadHandler* _handler, FOnSaveLoadComplete& _callback)
{
	UClass* Key = ARPGSaveGameType::StaticClass();
	if (nullptr == MapHandler.Find(Key))
		return;

	UARPGSaveGame* SaveGame = CreateGetSaveGameObject(Key);
	MapHandler[Key]->WriteSaveData(SaveGame);
	
	AsyncSaveGame(SaveGame, _callback);
}

template<typename ARPGSaveGameType>
inline void USaveLoadManager::LoadGame(FOnSaveLoadComplete& _callback)
{
	UClass* Key = ARPGSaveGameType::StaticClass();
	if (nullptr == MapHandler.Find(Key))
		return;

	AsyncLoadGame(Key, ARPGSaveGameType::SlotName, DEFAULT_SLOT_INDEX, _callback);
}

template<typename ARPGSaveGameType>
inline bool USaveLoadManager::DoesDataExist()
{
	return DoesSaveGameExist(ARPGSaveGameType::SlotName, DEFAULT_SLOT_INDEX);
}