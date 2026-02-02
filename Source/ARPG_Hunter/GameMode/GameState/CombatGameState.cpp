// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameState/CombatGameState.h"
#include "Define/Debug.h"

ACombatGameState::ACombatGameState()
{
}

FDelegateHandle ACombatGameState::SubscribeStageEvent(EStageEvent _event, TObjectPtr<UObject> _obj, const FName& _funcName)
{
	if (nullptr == StageEventBus.Find(_event))
		StageEventBus.Add(_event, FStageEvent());

	return StageEventBus[_event].AddUFunction(_obj, _funcName);
}

void ACombatGameState::UnsubscribeStageEvent(EStageEvent _event, const FDelegateHandle& _handle)
{
	ensure(_handle.IsValid() && StageEventBus.Find(_event));
	StageEventBus[_event].Remove(_handle);
}

void ACombatGameState::PublishStageEvent(EStageEvent _event, const FStageEventContext& _context)
{
	if (nullptr == StageEventBus.Find(_event))
	{
		UE_LOG(LogARPG, Warning, TEXT("Don't publish invalid stage event."));
		return;
	}

	StageEventBus[_event].Broadcast(_context);
}
