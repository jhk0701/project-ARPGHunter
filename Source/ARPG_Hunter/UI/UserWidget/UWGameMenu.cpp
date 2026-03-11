// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWGameMenu.h"
#include "Components/Button.h"



void UUWGameMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWGameMenu::HideUI);

	ExitButton->OnClicked.AddDynamic(this, &UUWGameMenu::ClickExit);
	ReturnButton->OnClicked.AddDynamic(this, &UUWGameMenu::ClickReturn);
}

void UUWGameMenu::ClickExit()
{
	HideUI();
	OnExitClicked.ExecuteIfBound();
}

void UUWGameMenu::ClickReturn()
{
	HideUI();
	OnReturnClicked.ExecuteIfBound();
}

void UUWGameMenu::ToggleOptionalButton(EButtonOption _opt, bool _bIsOn)
{
	switch (_opt)
	{
	case UUWGameMenu::EXIT:
		ExitButton->SetVisibility(_bIsOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		break;
	case UUWGameMenu::RETURN:
		ReturnButton->SetVisibility(_bIsOn ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		break;
	}
}
