// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageCutSceneSection.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

#include "Define/Debug.h"
#include "Core/GameMode/CombatGameMode.h"

void AStageCutSceneSection::BeginSection()
{
	// 플레이어 입장하는 시점에 컷씬 재생
	// 컷씬 데이터는 스테이지 데이터에 동봉
	// 컷씬이 없다면 재생하지 않음
	
	TObjectPtr<UWorld> World = GetWorld();

	TObjectPtr<ACombatGameMode> GM = World->GetAuthGameMode<ACombatGameMode>();
	TObjectPtr<ULevelSequence> CutSceneAsset = GM->GetCutSceneAsset(CutSceneIndex);
	if (nullptr == CutSceneAsset)
	{
		Super::BeginSection();
		return;
	}

	SetState(EState::IN_PROGRESS);

	FMovieSceneSequencePlaybackSettings PlaySetting;
	PlaySetting.bDisableLookAtInput = true;
	PlaySetting.bDisableMovementInput = true;
	PlaySetting.bHideHud = true;
	PlaySetting.bHidePlayer = true;

	ULevelSequencePlayer* Player = ULevelSequencePlayer::CreateLevelSequencePlayer(
		World,
		CutSceneAsset,
		PlaySetting,
		CutScenePlayer
	);

	Player->OnFinished.AddDynamic(this, &AStageCutSceneSection::OnCutSceneEnd);
}

void AStageCutSceneSection::OnCutSceneEnd()
{
	SpawnMonster();
}
