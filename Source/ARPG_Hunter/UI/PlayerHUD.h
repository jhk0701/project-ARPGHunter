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
	UPROPERTY()
	TSubclassOf<UUserWidget> PlayerUIClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> PlayerUI;
	UPROPERTY()
	TSubclassOf<UUserWidget> StageResultUIClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> StageResultUI;

	UPROPERTY()
	TSubclassOf<class ADamageFont> DamageUIClass;

public:
	APlayerHUD();
	TObjectPtr<UUserWidget> GetPlayerUI() { return PlayerUI; }
	void ShowResultUI(bool _bOnCleared);

protected:
	void BeginPlay() override;
};
