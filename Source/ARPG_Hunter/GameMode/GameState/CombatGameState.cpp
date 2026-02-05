// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameState/CombatGameState.h"
#include "GameMode/CombatGameMode.h"

void ACombatGameState::Init(uint8 _playerCnt, const TArray<struct FSection>& _section)
{
	PlayerCount = _playerCnt;
	bSectionCleared.SetNumZeroed(_section.Num());

	ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	GameMode->StageEvent[EStageEvent::PLAYER_DEAD].AddLambda(
		[this](const FStageEventContext& _context) 
		{
			OnPlayerDead.ExecuteIfBound(--PlayerCount);
		}
	);

	GameMode->StageEvent[EStageEvent::SECTION_CLEAR].AddLambda(
		[this](const FStageEventContext& _context) 
		{
			bSectionCleared[_context.SectionIndex] = true;
			OnSectionCleared.ExecuteIfBound(GameIsCleared());
		}
	);
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
