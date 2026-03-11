// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWTitleScreen.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"


void UUWTitleScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	NewGameButton->OnClicked.AddDynamic(this, &UUWTitleScreen::ClickNewGame);
	NameInputField->OnTextChanged.AddDynamic(this, &UUWTitleScreen::InputNameChanged);

	StartGameButton->OnClicked.AddDynamic(this, &UUWTitleScreen::ClickStartGame);
	ContinueButton->OnClicked.AddDynamic(this, &UUWTitleScreen::ClickContinue);
	BackToTitleButton->OnClicked.AddDynamic(this, &UUWTitleScreen::CloseNewGamePanel);
	ExitButton->OnClicked.AddDynamic(this, &UUWTitleScreen::ClickExitGame);
}

void UUWTitleScreen::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);

	CloseNewGamePanel();
}

void UUWTitleScreen::ShowContinueButton(bool _bIsShow)
{
	if (_bIsShow) 
	{
		ContinueButton->SetVisibility(ESlateVisibility::Visible);
		OverWriteWarningLabel->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ContinueButton->SetVisibility(ESlateVisibility::Collapsed);
		OverWriteWarningLabel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUWTitleScreen::ClickNewGame()
{
	NameInputField->SetText(FText::GetEmpty());
	StartGameButton->SetIsEnabled(false);
	NewGamePanel->SetVisibility(ESlateVisibility::Visible);
}

void UUWTitleScreen::InputNameChanged(const FText& _text)
{
	InputName = _text.ToString();
	if (InputName.Len() > MaxNameLength)
		InputName = InputName.Left(MaxNameLength);
	
	StartGameButton->SetIsEnabled(InputName.Len() > 0);
}

void UUWTitleScreen::ClickStartGame()
{
	OnClickNewGame.ExecuteIfBound();
}

void UUWTitleScreen::ClickContinue()
{
	OnClickContinue.ExecuteIfBound();
}

void UUWTitleScreen::CloseNewGamePanel()
{
	NewGamePanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UUWTitleScreen::ClickExitGame()
{
	OnClickExit.ExecuteIfBound();
}
