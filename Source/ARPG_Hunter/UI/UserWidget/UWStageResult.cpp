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

void UUWStageResult::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);

	if (OpenAnim)
		PlayAnimation(OpenAnim);
}

void UUWStageResult::Update(bool _bIsClear, const FStageData* _stageData)
{
	if (_bIsClear)
	{
		ResultLabel->SetText(FText::FromString(TEXT("Stage Clear")));
		RewardGoldLabel->SetText(FText::Format(FText::FromString(TEXT("{0} G")), _stageData->RewardGold));
		RewardExpLabel->SetText(FText::Format(FText::FromString(TEXT("{0} EXP")), _stageData->RewardExp));

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
		RewardExpLabel->SetText(FText::FromString(TEXT("- EXP")));
	}
}

void UUWStageResult::ClickReturnButton()
{
	HideUI();
	OnClickReturnButton.ExecuteIfBound();
}
