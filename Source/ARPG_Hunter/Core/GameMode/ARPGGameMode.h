// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARPGGameMode.generated.h"

class AAmbientSound;

/**
 * 
 */
UCLASS(Abstract)
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
	void BeginPlay() override;

public:
	void OpenLevel(const FName& _stageID);
	void BackToTown();
};
