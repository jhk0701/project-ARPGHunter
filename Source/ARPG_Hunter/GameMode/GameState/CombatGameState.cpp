// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameState/CombatGameState.h"
#include "Define/Debug.h"

ACombatGameState::ACombatGameState()
{
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
