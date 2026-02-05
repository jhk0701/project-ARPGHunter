// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CombatGameState.generated.h"

DECLARE_DELEGATE_OneParam(FOnPlayerDead, uint8);
DECLARE_DELEGATE_OneParam(FOnSectionCleared, bool);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ACombatGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	uint8 PlayerCount;

	UPROPERTY(VisibleAnywhere)
	TArray<bool> bSectionCleared;
public:
	FOnPlayerDead OnPlayerDead;
	FOnSectionCleared OnSectionCleared;

	void Init(uint8 _playerCnt, const TArray<struct FSection>& _section);
	bool GameIsCleared() const;
};
