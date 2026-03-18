// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWStageSelect.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Core/Subsystem/DataManager.h"
#include "Data/StageData.h"
#include "Data/ItemData.h"
#include "UI/UserWidget/UWItemSlot.h"


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
			ItemSlotContainer[i]->SetSize(ItemSlotSize);
			RewardItemContainer->AddChild(ItemSlotContainer[i]);
		}
	}

	Clear();
}

void UUWStageSelect::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);

	CurRegionID = FName(TEXT("1"));
	CurStageID = NAME_None;

	RefreshStageSlot();

	if (OpenAnim)
		PlayAnimation(OpenAnim);
}

void UUWStageSelect::ClickStartButton()
{
	if (CurStageID == NAME_None)
		return;

	OnClickStartButton.ExecuteIfBound(CurStageID);
	HideUI();
}

void UUWStageSelect::ClickStageSlot(uint8 _index)
{
	// 현재 지역 region의 _index에 해당하는 지역 ID가 선택된 것
	UDataManager* DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FRegionData* RegionData = DataManager->GetRegionData(CurRegionID);
	CurStageID = RegionData->StageIDs[_index];

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

	uint8 i = 0;
	// 필요한 슬롯 출력 및 업데이트
	for (; i < RegionData->StageIDs.Num(); ++i)
	{
		const FName& StageID = RegionData->StageIDs[i];
		FStageData* StageData = DataManager->GetStageData(StageID);
		if (i < StageSlotContainer.Num())
		{
			StageSlotContainer[i]->SetVisibility(ESlateVisibility::Visible);
			StageSlotContainer[i]->Update(StageData);
		}
	}

	// 쓰지 않는 슬롯 숨기기
	for (; i < StageSlotContainer.Num(); ++i)
		StageSlotContainer[i]->SetVisibility(ESlateVisibility::Hidden);
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
	GoldRewardLabel->SetText(FText::Format(FText::FromString(TEXT("{0} G")), StageData->RewardGold));
	ExpRewardLabel->SetText(FText::Format(FText::FromString(TEXT("{0} EXP")), StageData->RewardExp));
	
	// 보상 아이템 출력
	// 슬롯 보충이 필요한 경우
	if (ItemSlotContainer.Num() < StageData->RewardItems.Num())
	{
		// 슬롯 보충
		ItemSlotContainer.Reserve(StageData->RewardItems.Num());
		for (int i = 0; i < StageData->RewardItems.Num() - ItemSlotContainer.Num(); ++i)
			ItemSlotContainer.Add(CreateWidget<UUWItemSlot>(GetWorld(), ItemSlotClass));
	}

	// 필요한 슬롯 출력 및 업데이트
	uint8 i = 0;
	for (; i < StageData->RewardItems.Num(); ++i)
	{
		ItemSlotContainer[i]->SetVisibility(ESlateVisibility::Visible);
		
		FItemData* ItemData = DataManager->GetItemData(StageData->RewardItems[i].ID);
		ItemSlotContainer[i]->SetThumbnailOnly(ItemData->Item->Thumbnail);
	}

	// 미사용 슬롯 숨기기
	for (; i < ItemSlotContainer.Num(); ++i)
		ItemSlotContainer[i]->SetVisibility(ESlateVisibility::Hidden);
}

void UUWStageSelect::Clear()
{
	for (TObjectPtr<UUWStageSlot>& StageSlot : StageSlotContainer)
		StageSlot->SetVisibility(ESlateVisibility::Hidden);

	StageNameLabel->SetText(FText::FromString(TEXT("-")));
	StageDescLabel->SetText(FText::FromString(TEXT("-")));
	GoldRewardLabel->SetText(FText::FromString(TEXT("-")));
	ExpRewardLabel->SetText(FText::FromString(TEXT("-")));

	for (TObjectPtr<UUWItemSlot>& ItemSlot : ItemSlotContainer)
		ItemSlot->SetVisibility(ESlateVisibility::Hidden);
}
