// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameState/CombatGameState.h"

#include "Data/StageData.h"
#include "Core/GameMode/CombatGameMode.h"

void ACombatGameState::Init(uint8 _playerCnt, const TArray<FSection>& _section)
{
	PlayerCount = _playerCnt;
	bSectionCleared.SetNum(_section.Num());

	for (uint8 i = 0; i < _section.Num(); i++)
		bSectionCleared[i] = _section[i].Spawn.Num() == 0;

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
