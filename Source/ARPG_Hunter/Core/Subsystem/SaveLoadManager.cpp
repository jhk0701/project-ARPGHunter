// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Subsystem/SaveLoadManager.h"
#include "Interface/SaveLoadHandler.h"
#include "Kismet/GameplayStatics.h"

#include "Core/SaveGame/ARPGSaveGame.h"
#include "Core/Subsystem/PlayerManager.h"


UARPGSaveGame* USaveLoadManager::CreateGetSaveGameObject(UClass* _class)
{
	return Cast<UARPGSaveGame>(UGameplayStatics::CreateSaveGameObject(_class));
}

void USaveLoadManager::AsyncSaveGame(UARPGSaveGame* _savegame, FOnSaveComplete& _callback)
{
	FAsyncSaveGameToSlotDelegate Callback;
	Callback.BindWeakLambda(this,
		[_callback](const FString& _slotName, int32 _userIndex, bool _bIsIgnore)
		{
			_callback.ExecuteIfBound(_bIsIgnore);
		}
	);

	UGameplayStatics::AsyncSaveGameToSlot(_savegame, _savegame->SlotName, DEFAULT_SLOT_INDEX, Callback);
}
bool USaveLoadManager::DoesSaveGameExist(const FString& _slotName, int32 _slotIndex)
{
	return UGameplayStatics::DoesSaveGameExist(_slotName, DEFAULT_SLOT_INDEX);
}
//
//void USaveLoadManager::SavePlayerData(FOnSaveComplete _callback)
//{
//	//TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
//
//	//TObjectPtr<UPlayerSaveData> PlayerSave = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));
//	//PlayerSave->PlayerName = PlayerManager->GetPlayerName();
//	//PlayerSave->Gold = PlayerManager->GetGold();
//	//PlayerSave->SetInventoryData(PlayerManager->GetInventory());
//
//	//FAsyncSaveGameToSlotDelegate Callback;
//	//Callback.BindWeakLambda(this,
//	//	[_callback](const FString& _slotName, int32 _userIndex, bool _bIsIgnore)
//	//	{
//	//		_callback.ExecuteIfBound(_bIsIgnore);
//	//	}
//	//);
//
//	//UGameplayStatics::AsyncSaveGameToSlot(PlayerSave, PlayerSave->GetSlotName(), DEFAULT_SLOT_INDEX, Callback);
//}
//
//void USaveLoadManager::LoadPlayerData(FOnLoadComplete _callback)
//{
//	/*FAsyncLoadGameFromSlotDelegate Callback;
//	Callback.BindWeakLambda(this,
//		[_callback](const FString& _slotName, int32 _userIndex, USaveGame* _data)
//		{
//			_callback.ExecuteIfBound(_data);
//		}
//	);*/
//	// UGameplayStatics::AsyncLoadGameFromSlot(UPlayerSaveData::GetSlotName(), DEFAULT_SLOT_INDEX, Callback);
//	/*USaveGame* Data = UGameplayStatics::LoadGameFromSlot(UPlayerSaveGame::GetSlotName(), DEFAULT_SLOT_INDEX);
//	_callback.Execute(Data);*/
//}
//