// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/ARPGGameMode.h"
#include "TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ATitleGameMode : public AARPGGameMode
{
	GENERATED_BODY()

public:
	ATitleGameMode();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> TitleUIClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> TitleUIInst;
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void ClickNewGame();
	UFUNCTION()
	void ClickContinue();
};
