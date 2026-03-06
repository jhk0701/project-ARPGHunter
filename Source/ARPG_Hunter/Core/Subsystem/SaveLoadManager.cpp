// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystem/SaveLoadManager.h"
#include "Kismet/GameplayStatics.h"

#include "Core/SaveGame/ARPGSaveGame.h"
#include "Core/Subsystem/PlayerManager.h"

void USaveLoadManager::SavePlayerData(FOnSaveComplete _callback)
{
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

	TObjectPtr<UPlayerSaveData> PlayerSave = Cast<UPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveData::StaticClass()));
	PlayerSave->PlayerName = PlayerManager->GetPlayerName();
	PlayerSave->Gold = PlayerManager->GetGold();
	PlayerSave->SetInventoryData(PlayerManager->GetInventory());

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
	/*FAsyncLoadGameFromSlotDelegate Callback;
	Callback.BindWeakLambda(this,
		[_callback](const FString& _slotName, int32 _userIndex, USaveGame* _data)
		{
			_callback.ExecuteIfBound(_data);
		}
	);*/
	// UGameplayStatics::AsyncLoadGameFromSlot(UPlayerSaveData::GetSlotName(), DEFAULT_SLOT_INDEX, Callback);
	USaveGame* Data = UGameplayStatics::LoadGameFromSlot(UPlayerSaveData::GetSlotName(), DEFAULT_SLOT_INDEX);
	_callback.Execute(Data);
}

bool USaveLoadManager::DoesPlayerDataExist() const
{
	return UGameplayStatics::DoesSaveGameExist(UPlayerSaveData::GetSlotName(), DEFAULT_SLOT_INDEX);
}
