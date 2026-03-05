// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWTitleScreen.h"
#include "Components/Button.h"

void UUWTitleScreen::ShowContinueButton(bool _bIsShow)
{
	ContinueButton->SetVisibility(ESlateVisibility::Collapsed);
}
