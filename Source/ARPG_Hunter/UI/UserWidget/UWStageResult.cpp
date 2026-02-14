// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWStageResult.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Data/StageData.h"


void UUWStageResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReturnButton->OnClicked.AddDynamic(this, &UUWStageResult::ClickReturnButton);
}

void UUWStageResult::ShowUI()
{
	Super::ShowUI();

	if (OpenAnim)
		PlayAnimation(OpenAnim);
}


void UUWStageResult::Update(bool _bIsClear, const FStageData* _stageData)
{
	if (_bIsClear)
	{
		ResultLabel->SetText(FText::FromString(TEXT("Stage Clear")));
		RewardGoldLabel->SetText(FText::FromString(FString::Printf(TEXT("%d G"), _stageData->RewardGold)));
	}
	else
	{
		ResultLabel->SetText(FText::FromString(TEXT("Stage Fail")));
		RewardGoldLabel->SetText(FText::FromString(TEXT("- G")));
	}
}

void UUWStageResult::ClickReturnButton()
{
	OnClickReturnButton.ExecuteIfBound();
	HideUI();
}
