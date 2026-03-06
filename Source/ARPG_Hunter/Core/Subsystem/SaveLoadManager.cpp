// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystem/SaveLoadManager.h"
#include "Kismet/GameplayStatics.h"

#include "Core/SaveGame/ARPGSaveGame.h"

void USaveLoadManager::SavePlayerData(FPlayerDataParam& _param, FOnSaveComplete _callback)
{
	TObjectPtr<UPlayerSaveData> PlayerSave = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));
	PlayerSave->PlayerName = _param.PlayerName;
	PlayerSave->Gold = _param.Gold;
	PlayerSave->SetInventoryData(_param.Inventory);

	FAsyncSaveGameToSlotDelegate Callback;
	Callback.BindWeakLambda(this,
		[_callback](const FString& _slotName, int32 _userIndex, bool _bIsIgnore)
		{
			_callback.ExecuteIfBound(_bIsIgnore);
		}
	);

	UGameplayStatics::AsyncSaveGameToSlot(PlayerSave, PlayerSave->GetSlotName(), DEFAULT_SLOT_INDEX, Callback);
}

void USaveLoadManager::LoadPlayerData(FOnLoadComplete _callback)
{
	FAsyncLoadGameFromSlotDelegate Callback;
	Callback.BindWeakLambda(this,
		[_callback](const FString& _slotName, int32 _userIndex, USaveGame* _data)
		{
			_callback.ExecuteIfBound(_data);
		}
	);

	UGameplayStatics::AsyncLoadGameFromSlot(UPlayerSaveData::GetSlotName(), DEFAULT_SLOT_INDEX, Callback);
}

bool USaveLoadManager::DoesPlayerDataExist() const
{
	return UGameplayStatics::DoesSaveGameExist(UPlayerSaveData::GetSlotName(), DEFAULT_SLOT_INDEX);
}
