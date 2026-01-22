// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

private:
	TSubclassOf<UUserWidget> PlayerUIWidget;
	TObjectPtr<UUserWidget> PlayerUI;

public:
	APlayerHUD();

	TObjectPtr<UUserWidget> GetPlayerUI() { return PlayerUI; }

protected:
	void BeginPlay() override;
};
