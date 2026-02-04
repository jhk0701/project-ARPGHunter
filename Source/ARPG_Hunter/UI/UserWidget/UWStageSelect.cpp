// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWStageSelect.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "UI/UserWidget/UWItemSlot.h"
#include "Subsystem/DataManager/DataManager.h"
#include "Data/StageData.h"


void UUWStageSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SelectButton->OnClicked.AddDynamic(this, &UUWStageSlot::ClickStageButton);
}

void UUWStageSlot::Init(uint8 _index)
{
	Index = _index;
}

void UUWStageSlot::Update(FStageData* _data)
{
	if (nullptr == _data)
		return;

	StageNameLabel->SetText(FText::FromString(_data->StageName));
}

void UUWStageSlot::ClickStageButton()
{
	OnClickStageSlot.ExecuteIfBound(Index);
}

void UUWStageSelect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWStageSelect::HideUI);
	StartButton->OnClicked.AddDynamic(this, &UUWStageSelect::ClickStartButton);

	if(StageSlotClass)
	{
		StageSlotContainer.SetNum(InitStageSlotSize);
		for (int i = 0; i < InitStageSlotSize; ++i)
		{
			StageSlotContainer[i] = CreateWidget<UUWStageSlot>(GetWorld(), StageSlotClass);
			StageSlotContainer[i]->Init(i);
			StageSlotContainer[i]->OnClickStageSlot.BindUObject(this, &UUWStageSelect::ClickStageSlot);

			StageList->AddChild(StageSlotContainer[i]);
		}
	}

	if (ItemSlotClass)
	{
		ItemSlotContainer.SetNum(InitRewardItemSlotSize);
		for (int i = 0; i < InitRewardItemSlotSize; ++i)
		{
			ItemSlotContainer[i] = CreateWidget<UUWItemSlot>(GetWorld(), ItemSlotClass);
			RewardItemContainer->AddChild(ItemSlotContainer[i]);
		}
	}
}

void UUWStageSelect::ShowUI()
{
	CurRegionID = FName(TEXT("1"));
	Refresh();
	AddToViewport();
}

void UUWStageSelect::HideUI()
{
	RemoveFromViewport();
}

void UUWStageSelect::ClickStartButton()
{
	OnClickStartButton.ExecuteIfBound(CurStageID);
}

void UUWStageSelect::ClickStageSlot(uint8 _index)
{
	// 현재 지역 region의 _index에 해당하는 지역 ID가 선택된 것
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FRegionData* RegionData = DataManager->GetRegionData(CurRegionID);
	CurStageID = RegionData->StageIDs[_index];

	RefreshStageInfo();
}

void UUWStageSelect::Refresh()
{
	RefreshStageSlot();
	RefreshStageInfo();
}

void UUWStageSelect::RefreshStageSlot()
{
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FRegionData* RegionData = DataManager->GetRegionData(CurRegionID);

	if (nullptr == RegionData)
	{
		Clear();
		return;
	}

	if (StageSlotContainer.Num() < RegionData->StageIDs.Num())
	{
		// 슬롯 보충
		StageSlotContainer.Reserve(RegionData->StageIDs.Num());
		for (int i = 0; i < RegionData->StageIDs.Num() - StageSlotContainer.Num(); ++i)
		{
			StageSlotContainer.Add(CreateWidget<UUWStageSlot>(GetWorld(), StageSlotClass));
			TObjectPtr<UUWStageSlot> LastSlot = StageSlotContainer.Last();
			LastSlot->Init(StageSlotContainer.Num() - 1);
			LastSlot->OnClickStageSlot.BindUObject(this, &UUWStageSelect::ClickStageSlot);
		}
	}

	for (int i = 0; i < RegionData->StageIDs.Num(); ++i)
	{
		FName StageID = RegionData->StageIDs[i];
		FStageData* StageData = DataManager->GetStageData(StageID);
		if (i < StageSlotContainer.Num())
		{
			StageSlotContainer[i]->SetVisibility(ESlateVisibility::Visible);
			StageSlotContainer[i]->Update(StageData);
		}
	}
}

void UUWStageSelect::RefreshStageInfo()
{
	// 스테이지 정보 출력
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FStageData* StageData = DataManager->GetStageData(CurStageID);

	if (nullptr == StageData)
		return;

	StageNameLabel->SetText(FText::FromString(StageData->StageName));
	StageDescLabel->SetText(FText::FromString(StageData->StageDesc));
	GoldRewardLabel->SetText(FText::FromString(FString::Printf(TEXT("%d G"), StageData->RewardGold)));
	
	// 보상 아이템 출력
	// 슬롯 보충이 필요한 경우
	if (ItemSlotContainer.Num() < StageData->RewardItems.Num())
	{
		// 슬롯 보충
		ItemSlotContainer.Reserve(StageData->RewardItems.Num());
		for (int i = 0; i < StageData->RewardItems.Num() - ItemSlotContainer.Num(); ++i)
			ItemSlotContainer.Add(CreateWidget<UUWItemSlot>(GetWorld(), ItemSlotClass));
	}

	for (uint8 i = 0; i < StageData->RewardItems.Num(); ++i)
	{
		ItemSlotContainer[i]->SetVisibility(ESlateVisibility::Visible);
		// TODO : 아이템 기능 만들고 나서 추가해줄 것
		// ItemSlotContainer[i]->uyp
	}
}

void UUWStageSelect::Clear()
{
	for (TObjectPtr<UUWStageSlot>& StageSlot : StageSlotContainer)
		StageSlot->SetVisibility(ESlateVisibility::Hidden);

	StageNameLabel->SetText(FText::FromString(TEXT("-")));
	StageDescLabel->SetText(FText::FromString(TEXT("-")));
	GoldRewardLabel->SetText(FText::FromString(TEXT("-")));

	for (TObjectPtr<UUWItemSlot>& ItemSlot : ItemSlotContainer)
		ItemSlot->SetVisibility(ESlateVisibility::Hidden);
}
