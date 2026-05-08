// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UI/UserWidget/UWGameMenu.h"

#include "Core/GameMode/ARPGGameMode.h"

APlayerHUD::APlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false; 

	static ConstructorHelpers::FClassFinder<UUWPopUp> InputGuideClassFinder(TEXT("/Game/06-UI/WBP_InputGuide.WBP_InputGuide_C"));
	if (InputGuideClassFinder.Succeeded())
		InputGuideUIClass = InputGuideClassFinder.Class;

	static ConstructorHelpers::FClassFinder<UUWPopUp> GameMenuClassFinder(TEXT("/Game/06-UI/WBP_GameMenu.WBP_GameMenu_C"));
	if (GameMenuClassFinder.Succeeded())
		GameMenuUIClass = GameMenuClassFinder.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	InitInputGuideUI();
	InitMenuUI();
}

void APlayerHUD::InitInputGuideUI()
{
	TryCreateWidget<UUWPopUp>(InputGuideUIClass, InputGuideUI);
}

void APlayerHUD::InitMenuUI()
{
	if (false == TryCreateWidget<UUWPopUp>(GameMenuUIClass, GameMenuUI))
		return;

	TObjectPtr<UUWGameMenu> MenuUI = Cast<UUWGameMenu>(GameMenuUI);

	if (nullptr == MenuUI)
		return;

	MenuUI->OnExitClicked.BindLambda(
		[this]()
		{
			AARPGGameMode* GM = Cast<AARPGGameMode>(GetWorld()->GetAuthGameMode());
			if (GM == nullptr)
				return;

			GM->SaveGame(
				[this]()
				{
					AARPGGameMode* GM = Cast<AARPGGameMode>(GetWorld()->GetAuthGameMode());
					GM->ExitGame();
				}
			);
		}
	);
}

TWeakObjectPtr<UUWPopUp> APlayerHUD::GetGameMenuUI() const
{
	return GameMenuUI;
}

void APlayerHUD::ToggleGameMenuUI()
{
	if (nullptr == GameMenuUI)
		return;

	if (GameMenuUI->IsShowing())
		GameMenuUI->HideUI();
	else
		GameMenuUI->ShowUI();
}

void APlayerHUD::ToggleInputGuideUI()
{
	if (nullptr == InputGuideUI)
		return;

	if (InputGuideUI->IsShowing())
		InputGuideUI->HideUI();
	else
		InputGuideUI->ShowUI();
}
