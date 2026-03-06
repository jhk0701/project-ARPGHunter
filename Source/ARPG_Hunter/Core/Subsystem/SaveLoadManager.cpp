// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Subsystem/SaveLoadManager.h"
#include "Interface/SaveLoadHandler.h"
#include "Kismet/GameplayStatics.h"

#include "SaveGame/ARPGSaveGame.h"

USaveLoadManager::USaveLoadManager()
{
	MapHandler.Empty();
}

UARPGSaveGame* USaveLoadManager::CreateGetSaveGameObject(UClass* _class)
{
	return Cast<UARPGSaveGame>(UGameplayStatics::CreateSaveGameObject(_class));
}

void USaveLoadManager::AsyncSaveGame(UARPGSaveGame* _savegame, FOnSaveLoadComplete& _callback)
{
	FAsyncSaveGameToSlotDelegate Callback;
	Callback.BindWeakLambda(this,
		[_callback](const FString& _slotName, int32 _userIndex, bool _bIsIgnore)
		{
			_callback.ExecuteIfBound();
		}
	);

	UGameplayStatics::AsyncSaveGameToSlot(_savegame, _savegame->SlotName, DEFAULT_SLOT_INDEX, Callback);
}

void USaveLoadManager::AsyncLoadGame(UClass* _key, const FString& _slotName, int32 _slotIndex, FOnSaveLoadComplete& _callback)
{
	FAsyncLoadGameFromSlotDelegate Callback;
	Callback.BindWeakLambda(this,
		[this, _key, _callback](const FString& _slotName, int32 _userIndex, USaveGame* _data)
		{
			TObjectPtr<UARPGSaveGame> SaveGame = Cast<UARPGSaveGame>(_data);
			MapHandler[_key]->ReadSaveData(SaveGame);

			_callback.ExecuteIfBound();
		}
	);

	UGameplayStatics::AsyncLoadGameFromSlot(_slotName, DEFAULT_SLOT_INDEX, Callback);
}

bool USaveLoadManager::DoesSaveGameExist(const FString& _slotName, int32 _slotIndex)
{
	return UGameplayStatics::DoesSaveGameExist(_slotName, DEFAULT_SLOT_INDEX);
}

void USaveLoadManager::SaveAll(FOnSaveLoadComplete& _callback)
{
	for (const TPair<TSubclassOf<UARPGSaveGame>, ISaveLoadHandler*>& Pair : MapHandler)
	{
		UARPGSaveGame* SaveGame = CreateGetSaveGameObject(Pair.Key);
		MapHandler[Pair.Key]->WriteSaveData(SaveGame);

		AsyncSaveGame(SaveGame, _callback);
	}
}