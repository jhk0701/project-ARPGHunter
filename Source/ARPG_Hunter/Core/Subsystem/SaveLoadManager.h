// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interface/SaveLoadHandler.h"
#include "SaveLoadManager.generated.h"

class UARPGSaveGame;

DECLARE_DELEGATE_OneParam(FOnSaveComplete, bool);
DECLARE_DELEGATE_OneParam(FOnLoadComplete, TObjectPtr<UARPGSaveGame>);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API USaveLoadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
private:
	const int32 DEFAULT_SLOT_INDEX = 0;
	
	TMap<TSubclassOf<UARPGSaveGame>, ISaveLoadHandler*> SaveLoadHandler;

	UARPGSaveGame* CreateGetSaveGameObject(UClass* _class);
	void AsyncSaveGame(UARPGSaveGame* _savegame, FOnSaveComplete& _callback);
	bool DoesSaveGameExist(const FString& _slotName, int32 _slotIndex);

public:
	template<typename ARPGSaveGameType>
	void RegisterHandler(ISaveLoadHandler* _handler);
	
	template<typename ARPGSaveGameType>
	void SaveGame(FOnSaveComplete& _callback);
	
	template<typename ARPGSaveGameType>
	bool DoesDataExist();

	/*void SavePlayerData(FOnSaveComplete _callback);
	void LoadPlayerData(FOnLoadComplete _callback);
	bool DoesPlayerDataExist() const;*/
};

template<typename ARPGSaveGameType>
inline void USaveLoadManager::RegisterHandler(ISaveLoadHandler* _handler)
{
	UClass* Key = ARPGSaveGameType::StaticClass();
	if (SaveLoadHandler.Find(Key) == nullptr)
		SaveLoadHandler.Add(Key, _handler);
	else
		SaveLoadHandler[Key] = _handler;
}

template<typename ARPGSaveGameType>
inline void USaveLoadManager::SaveGame(FOnSaveComplete& _callback)
{
	UClass* Key = ARPGSaveGameType::StaticClass();
	if (nullptr == SaveLoadHandler.Find(Key))
		return;

	UARPGSaveGame* SaveGame = CreateGetSaveGameObject(Key);
	SaveLoadHandler[Key]->WriteSaveData(SaveGame);

	AsyncSaveGame(SaveGame, _callback);
}

template<typename ARPGSaveGameType>
inline bool USaveLoadManager::DoesDataExist()
{
	return DoesSaveGameExist(ARPGSaveGameType::SlotName, DEFAULT_SLOT_INDEX);
}