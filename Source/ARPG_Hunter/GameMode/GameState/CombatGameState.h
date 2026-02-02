// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CombatGameState.generated.h"

UENUM()
enum class EStageEvent : uint8
{
	HUNT		UMETA(DisplayName = "Hunt"),
	COLLECT		UMETA(DisplayName = "Collect"),

	END			UMETA(Hidden)
};

struct FStageEventContext 
{
	TObjectPtr<UObject> Target;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FStageEvent, const FStageEventContext&);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ACombatGameState : public AGameStateBase
{
	GENERATED_BODY()
	
private:
	TMap<EStageEvent, FStageEvent> StageEventBus;

public:
	ACombatGameState();

	FStageEvent& GetEvent(EStageEvent _event) { return StageEventBus[_event]; }
	void PublishStageEvent(EStageEvent _event, const FStageEventContext& _context);
};
