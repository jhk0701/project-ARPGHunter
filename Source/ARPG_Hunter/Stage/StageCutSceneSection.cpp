// Fill out your copyright notice in the Description page of Project Settings.


#include "Stage/StageCutSceneSection.h"
#include "Engine/AssetManager.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

#include "Data/CutSceneAssetData.h"
#include "Define/Debug.h"
#include "Core/GameMode/CombatGameMode.h"
#include "UI/CombatHUD.h"
#include "UI/UserWidget/UWCutScene.h"


AStageCutSceneSection::AStageCutSceneSection()
{
	static ConstructorHelpers::FClassFinder<UUWCutScene> CutSceneUIFinder(TEXT("/Game/06-UI/WBP_CutScene.WBP_CutScene_C"));
	if (CutSceneUIFinder.Succeeded())
		CutSceneUIClass = CutSceneUIFinder.Class;
}

void AStageCutSceneSection::BeginPlay()
{
	Super::BeginPlay();

	if (CutSceneUIClass)
	{
		CutSceneUI = CreateWidget<UUWCutScene>(GetWorld(), CutSceneUIClass);
		CutSceneUI->OnSkipClicked.BindUObject(this, &AStageCutSceneSection::SkipCutScene);
	}

	TObjectPtr<ACombatGameMode> GM = GetWorld()->GetAuthGameMode<ACombatGameMode>();
	TObjectPtr<UCutSceneAssetData> CutSceneAssetData = GM->GetCutSceneAsset(CutSceneIndex);
	if (nullptr == CutSceneAssetData)
		return;

	CutSceneAssetPath = CutSceneAssetData->LevelSequence;
	
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(
		CutSceneAssetPath.ToSoftObjectPath(),
		FStreamableDelegate::CreateUObject(this, &AStageCutSceneSection::OnCutSceneAssetLoaded)
	);
}

void AStageCutSceneSection::OnCutSceneAssetLoaded()
{
	CutSceneAsset = CutSceneAssetPath.Get();
}

void AStageCutSceneSection::BeginSection()
{
	// 플레이어 입장하는 시점에 컷씬 재생
	TObjectPtr<UWorld> World = GetWorld();
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

	TObjectPtr<ULevelSequencePlayer> SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
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

	// PlayerHUD 가리기
	TObjectPtr<ACombatHUD> HUD = World->GetFirstPlayerController()->GetHUD<ACombatHUD>();
	HUD->ShowPlayerUI(false);

	if (CutSceneUI)
		CutSceneUI->ShowUI();

	SequencePlayer->OnFinished.AddDynamic(this, &AStageCutSceneSection::OnCutSceneEnd);
	SequencePlayer->Play();
}


void AStageCutSceneSection::OnCutSceneEnd()
{
	if (CutSceneUI->IsShowing())
		CutSceneUI->HideUI();

	TObjectPtr<ACombatHUD> HUD = GetWorld()->GetFirstPlayerController()->GetHUD<ACombatHUD>();
	HUD->ShowPlayerUI(true);

	// 완료 시, 몬스터 스폰
	SpawnMonster();

	CutScenePlayer->Destroy();
	CutScenePlayer = nullptr;
}

void AStageCutSceneSection::SkipCutScene()
{
	TObjectPtr<ULevelSequencePlayer> SequencePlayer = CutScenePlayer->GetSequencePlayer();
	SequencePlayer->Stop();
	SequencePlayer->OnFinished.Broadcast();
}
