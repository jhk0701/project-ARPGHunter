// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/UserWidget/UWPlayerHUD.h"

#include "Subsystem/ObjectPool/ObjectPoolManager.h"
#include "UI/Actor/DamageFont.h"
#include "UI/UserWidget/UWStageResult.h"
#include "GameMode/CombatGameMode.h"

APlayerHUD::APlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false; 

	// 플레이어 HUD
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerUIFinder(TEXT("/Game/06-UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
	if (PlayerUIFinder.Succeeded())
		PlayerUIClass = PlayerUIFinder.Class;

	// 스테이지 결과 UI
	static ConstructorHelpers::FClassFinder<UUserWidget> StageResultUIFinder(TEXT("/Game/06-UI/WBP_StageResult.WBP_StageResult_C"));
	if (StageResultUIFinder.Succeeded())
		StageResultUIClass = StageResultUIFinder.Class;

	static ConstructorHelpers::FClassFinder<AActor> DamageUIFinder(TEXT("/Game/02-BP/DamageFont/BP_DamageFont.BP_DamageFont_C"));
	if (DamageUIFinder.Succeeded())
		DamageUIClass = DamageUIFinder.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerUIClass)
	{
		PlayerUI = CreateWidget<UUWPlayerHUD>(GetWorld(), PlayerUIClass);
		if (PlayerUI)
			PlayerUI->AddToViewport();
	}

	if (StageResultUIClass)
	{
		StageResultUI = CreateWidget<UUserWidget>(GetWorld(), StageResultUIClass);

		if (ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>())
			GameMode->OnGameEnd.AddUObject(this, &APlayerHUD::ShowResultUI);
	}

	if (DamageUIClass)
	{
		UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
		ObjectPool->Register(ADamageFont::StaticClass(), [this]() { return GetWorld()->SpawnActor(DamageUIClass); }, 10);
	}
}

void APlayerHUD::ShowResultUI(bool _bOnCleared)
{
	UUWStageResult* ResultUI = Cast<UUWStageResult>(StageResultUI);

	ResultUI->Update(_bOnCleared);
	ResultUI->OnClickReturnButton.BindLambda(
		[this]() 
		{
			if (ACombatGameMode* GameMode = GetWorld()->GetAuthGameMode<ACombatGameMode>())
				GameMode->BackToTown();
		}
	);

	ResultUI->ShowUI();
}
