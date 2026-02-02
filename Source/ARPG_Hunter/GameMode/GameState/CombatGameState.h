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

	FDelegateHandle SubscribeStageEvent(EStageEvent _event, TObjectPtr<UObject> _obj, const FName& _funcName);
	void UnsubscribeStageEvent(EStageEvent _event, const FDelegateHandle& _handle);
	void PublishStageEvent(EStageEvent _event, const FStageEventContext& _context);
};
