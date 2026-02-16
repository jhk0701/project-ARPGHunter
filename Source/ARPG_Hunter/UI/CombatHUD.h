// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PlayerHUD.h"
#include "CombatHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ACombatHUD : public APlayerHUD
{
	GENERATED_BODY()
public:
	ACombatHUD();
	
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

protected:
	void BeginPlay() override;

public:
	TObjectPtr<UUserWidget> GetPlayerUI() { return PlayerUI; }
	void ShowResultUI(bool _bOnCleared, const struct FStageData* _stageData);
};
