// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageCutSceneSection.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"

#include "Define/Debug.h"
#include "Core/GameMode/CombatGameMode.h"

void AStageCutSceneSection::BeginSection()
{
	// 플레이어 입장하는 시점에 컷씬 재생
	TObjectPtr<UWorld> World = GetWorld();
	TObjectPtr<ACombatGameMode> GM = World->GetAuthGameMode<ACombatGameMode>();
	TObjectPtr<ULevelSequence> CutSceneAsset = GM->GetCutSceneAsset(CutSceneIndex);
	if (nullptr == CutSceneAsset)
	{
		// 컷씬이 없다면 재생하지 않고, Section과 동일하게 처리
		Super::BeginSection();
		return;
	}

	SetState(EState::IN_PROGRESS);

	FMovieSceneSequencePlaybackSettings PlaySetting;
	PlaySetting.bDisableLookAtInput = true;
	PlaySetting.bDisableMovementInput = true;
	PlaySetting.bHideHud = true;
	PlaySetting.bHidePlayer = true;

	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		World,
		CutSceneAsset,
		PlaySetting,
		CutScenePlayer
	);

	if (nullptr == SequencePlayer)
	{
		Super::BeginSection();
		return;
	}

	SequencePlayer->OnFinished.AddDynamic(this, &AStageCutSceneSection::OnCutSceneEnd);
	SequencePlayer->Play();
}

void AStageCutSceneSection::OnCutSceneEnd()
{
	// 완료 시, 몬스터 스폰
	SpawnMonster();
}
