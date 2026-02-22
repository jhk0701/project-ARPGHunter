// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/TestGameMode.h"

void ATestGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 테스트 모드 강제 할당
	SetStageData(TEXT("0"));
}