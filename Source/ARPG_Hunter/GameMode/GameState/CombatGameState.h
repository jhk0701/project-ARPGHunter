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
DECLARE_DELEGATE(FOnStageCleared);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ACombatGameState : public AGameStateBase
{
	GENERATED_BODY()

private:
	TArray<bool> bSectionCleared;;

public:
	ACombatGameState();
	
	TMap<EStageEvent, FStageEvent> StageEventBus;
	FOnStageCleared OnStageCleared;
	
	void PublishEvent(EStageEvent _event, const FStageEventContext& _context)
	{
		StageEventBus[_event].Broadcast(_context);
	}

	void Init(const TArray<struct FSection>& _section);
	void SetSectionClear(uint8 _id);
	bool GameIsCleared() const;
};
