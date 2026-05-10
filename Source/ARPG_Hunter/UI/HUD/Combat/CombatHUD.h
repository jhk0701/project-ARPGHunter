// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/PlayerHUD.h"
#include "CombatHUD.generated.h"

class UUWCombatHUD;
class UUWStageResult;

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
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWCombatHUD> PlayerUIClass;
	UPROPERTY()
	TObjectPtr<UUWCombatHUD> PlayerUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWStageResult> StageResultUIClass;
	UPROPERTY()
	TObjectPtr<UUWStageResult> StageResultUI;

	UPROPERTY()
	TSubclassOf<class ADamageFont> DamageUIClass;

public:
	TObjectPtr<UUWCombatHUD> GetPlayerUI() const { return PlayerUI; }

	void ShowPlayerUI(bool _bIsShow);
	void ShowResultUI(bool _bOnCleared, const struct FStageData* _stageData);

protected:
	void BeginPlay() override;
	void InitMenuUI() override;

private:
	void InitPlayerUI();
	void InitStageResultUI();
	void InitDamageUI();

};
