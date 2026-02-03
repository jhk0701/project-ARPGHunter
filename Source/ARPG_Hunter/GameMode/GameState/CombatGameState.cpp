// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameState/CombatGameState.h"

ACombatGameState::ACombatGameState()
{
	for (uint8 i = 0; i < static_cast<uint8>(EStageEvent::END); ++i)
		StageEventBus.Add(static_cast<EStageEvent>(i));
}

void ACombatGameState::Init(uint8 _playerCnt, const TArray<struct FSection>& _section)
{
	PlayerCount = _playerCnt;
	bSectionCleared.SetNumZeroed(_section.Num());

	StageEventBus[EStageEvent::PLAYER_DEAD].AddLambda(
		[this](const FStageEventContext& _context) 
		{
			PlayerCount--;

			if(PlayerCount == 0)
				OnStageFailed.ExecuteIfBound();
		}
	);
}

void ACombatGameState::SetSectionClear(uint8 _id)
{
	bSectionCleared[_id] = true;

	if (GameIsCleared())
		OnStageCleared.ExecuteIfBound();
}

bool ACombatGameState::GameIsCleared() const
{
	for (bool bCleared : bSectionCleared)
	{
		if (!bCleared)
			return false;
	}
	return true;
}
