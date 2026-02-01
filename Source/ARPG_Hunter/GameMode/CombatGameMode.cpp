// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/CombatGameMode.h"

#include "Controller/PlayerCombatController.h"
#include "UI/PlayerHUD.h"

#include "Core/ARPGGameInstance.h"
#include "Subsystem/DataManager/DataManager.h"
#include "Data/StageData.h"

ACombatGameMode::ACombatGameMode()
{
	PlayerControllerClass = APlayerCombatController::StaticClass();
	HUDClass = APlayerHUD::StaticClass();
}


void ACombatGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 게임 인스턴스에 저장된 스테이지 ID 데이터 읽어오기
	UARPGGameInstance* GI = Cast<UARPGGameInstance>(GetGameInstance());
	check(nullptr != GI);

	if (GI->GetStageID().IsNone())
	{
		StageData = GI->GetSubsystem<UDataManager>()->GetStageData(FName(TEXT("0")));
		return;
	}

	StageData = GI->GetSubsystem<UDataManager>()->GetStageData(GI->GetStageID());
}

void ACombatGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 게임 진행 준비
	bSectionIsCleared.SetNum(StageData->Sections.Num());
	for (int i = 0; i < StageData->Sections.Num(); ++i)
		bSectionIsCleared[i] = false;
}

const FSection& ACombatGameMode::GetSection(uint8 _idx) const
{
	check(StageData);
	return StageData->Sections[_idx];
}