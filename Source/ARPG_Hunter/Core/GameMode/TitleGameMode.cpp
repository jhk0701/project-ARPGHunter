// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameMode/TitleGameMode.h"

#include "UI/UserWidget/UWTitleScreen.h"

ATitleGameMode::ATitleGameMode()
{
	DefaultPawnClass = nullptr;
	HUDClass = nullptr;

	static ConstructorHelpers::FClassFinder<UUserWidget> TitleUIFinder(TEXT("/Game/06-UI/WBP_Title.WBP_Title_C"));
	if (TitleUIFinder.Succeeded())
		TitleUIClass = TitleUIFinder.Class;
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (TitleUIClass == nullptr)
		return;

	TitleUIInst = CreateWidget<UUserWidget>(GetWorld(), TitleUIClass);

	TObjectPtr<UUWTitleScreen> TitleUI = Cast<UUWTitleScreen>(TitleUIInst);
	if (TitleUI == nullptr)
		return;

	// TitleUI->OnClickNewGame.BindUObject();
	// TitleUI->OnClickContinue.BindUObject();
	TitleUI->OnClickExit.BindUObject(this, &ATitleGameMode::ExitGame);

	TitleUI->ShowUI();
}
