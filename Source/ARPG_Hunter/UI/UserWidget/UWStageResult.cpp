// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWStageResult.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"

#include "Data/StageData.h"
#include "UI/UserWidget/UWItemSlot.h"

void UUWStageResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ReturnButton->OnClicked.AddDynamic(this, &UUWStageResult::ClickReturnButton);
}

void UUWStageResult::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);

	if (OpenAnim)
		PlayAnimation(OpenAnim);
}


void UUWStageResult::Update(bool _bIsClear, const FStageData* _stageData)
{
	if (_bIsClear)
	{
		ResultLabel->SetText(FText::FromString(TEXT("Stage Clear")));

		FText GoldFormat = FText::FromString(TEXT("{0} G"));
		RewardGoldLabel->SetText(FText::Format(GoldFormat, _stageData->RewardGold));

		if (ItemSlotClass)
		{
			for (const FRewardItem& RewardItem : _stageData->RewardItems)
			{
				TObjectPtr<UUWItemSlot> SlotInst = CreateWidget<UUWItemSlot>(GetWorld(), ItemSlotClass);
				SlotInst->SetSlotUsingID(RewardItem.ID, RewardItem.Count);
				SlotInst->SetSize(SlotSize);
				RewardItemContainer->AddChild(SlotInst);
			}
		}
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
