// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWStageResult.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"


void UUWStageResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReturnButton->OnClicked.AddDynamic(this, &UUWStageResult::ClickReturnButton);
}


void UUWStageResult::ShowUI()
{
	AddToViewport();

	if (OpenAnim)
		PlayAnimation(OpenAnim);
}

void UUWStageResult::HideUI()
{
	RemoveFromParent();
}

void UUWStageResult::Update(bool _bIsClear)
{
	ResultLabel->SetText(_bIsClear ? FText::FromString(TEXT("Stage Clear")) : FText::FromString(TEXT("Stage Fail")));
}

void UUWStageResult::ClickReturnButton()
{
	OnClickReturnButton.ExecuteIfBound();
}
