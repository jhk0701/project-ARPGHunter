// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/ARPGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"
#include "Components/AudioComponent.h"

#include "Core/ARPGGameInstance.h"
#include "Core/Subsystem/SaveLoadManager/SaveLoadManager.h"
#include "Core/Subsystem/DataManager/DataManager.h"
#include "Data/StageData.h"
#include "Player/PlayerCharacter.h"

AARPGGameMode::AARPGGameMode()
{
	static ConstructorHelpers::FClassFinder<ACharacter> PlayerFinder(TEXT("/Game/02-BP/Player/BP_Player.BP_Player_C"));
	if (PlayerFinder.Succeeded())
		DefaultPawnClass = PlayerFinder.Class;

	static ConstructorHelpers::FClassFinder<AAmbientSound> BgmPlayerFinder(TEXT("/Game/02-BP/Sound/BP_BGMPlayer.BP_BGMPlayer_C"));
	if (BgmPlayerFinder.Succeeded())
		BgmPlayerClass = BgmPlayerFinder.Class;
}

void AARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (BgmPlayerClass)
	{
		BgmPlayer = GetWorld()->SpawnActor<AAmbientSound>(BgmPlayerClass);

		TObjectPtr<UARPGGameInstance> GI = GetGameInstance<UARPGGameInstance>();
		if (GI->GetStageID().IsNone())
			GI->SetStageID(TEXT("0"));

		FStageData* StageData = GI->GetSubsystem<UDataManager>()->GetStageData(GI->GetStageID());
		if (StageData->BgmSource)
		{
			TObjectPtr<UAudioComponent> Audio = BgmPlayer->GetAudioComponent();
			Audio->SetSound(StageData->BgmSource);
			Audio->Play();
		}
	}
}

void AARPGGameMode::SaveGame(TFunction<void()> _callback)
{
	FOnSaveLoadComplete CompleteCallback;

	if (_callback)
		CompleteCallback.BindLambda(_callback);

	GetGameInstance()->GetSubsystem<USaveLoadManager>()->SaveAll(CompleteCallback);
}

void AARPGGameMode::ExitGame()
{
	UWorld* World = GetWorld();
	UKismetSystemLibrary::QuitGame(World, World->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void AARPGGameMode::OpenLevel(const FName& _stageID)
{
	TObjectPtr<UARPGGameInstance> GI = GetGameInstance<UARPGGameInstance>();
	FStageData* StageData = GI->GetSubsystem<UDataManager>()->GetStageData(_stageID);

	if (nullptr == StageData)
	{
		FName TownID = FName(TEXT("1"));
		StageData = GI->GetSubsystem<UDataManager>()->GetStageData(TownID);
		GI->SetStageID(TownID);
	}
	else
		GI->SetStageID(_stageID);
	
	UGameplayStatics::OpenLevel(GetWorld(), StageData->LevelToLoad);
}

void AARPGGameMode::GoToTown()
{
	OpenLevel(FName(TEXT("1")));
}
