// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameMode.generated.h"

class AAmbientSound;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API AARPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARPGGameMode();

private:
	UPROPERTY(EditAnywhere, Category = "Sound|BGM")
	TSubclassOf<AAmbientSound> BgmPlayerClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Sound|BGM")
	TObjectPtr<AAmbientSound> BgmPlayer;

protected:
	virtual void BeginPlay() override;

public:
	virtual void SaveGame(TFunction<void()> _callback = nullptr);
	UFUNCTION()
	virtual void ExitGame();

	void OpenLevel(const FName& _stageID);

	UFUNCTION()
	void GoToTown();
};
