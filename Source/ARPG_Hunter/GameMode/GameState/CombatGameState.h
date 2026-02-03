// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CombatGameState.generated.h"

UENUM()
enum class EStageEvent : uint8
{
	HUNT			UMETA(DisplayName = "Hunt"),
	PLAYER_DEAD		UMETA(DisplayName = "PlayerDead"),

	END				UMETA(Hidden)
};

struct FStageEventContext 
{
	uint8 SectionIndex;
	TObjectPtr<UObject> Target;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FStageEvent, const FStageEventContext&);
DECLARE_DELEGATE(FStageEndEvent);

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
	ACombatGameState();
	
	TMap<EStageEvent, FStageEvent> StageEventBus;
	FStageEndEvent OnStageCleared;
	FStageEndEvent OnStageFailed;
	
	void PublishEvent(EStageEvent _event, const FStageEventContext& _context)
	{
		StageEventBus[_event].Broadcast(_context);
	}

	void Init(uint8 _playerCnt, const TArray<struct FSection>& _section);
	void SetSectionClear(uint8 _id);
	bool GameIsCleared() const;
};
