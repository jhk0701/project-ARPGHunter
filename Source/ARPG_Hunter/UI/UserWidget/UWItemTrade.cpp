// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemTrade.h"
#include "Components/Button.h"

#include "Core/Subsystem/DataManager.h"
#include "Data/ItemTradeData.h"

#include "UI/UserWidget/UWItemSlot.h"
#include "UI/UserWidget/UWItemDetail.h"

void UUWItemTrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWItemTrade::HideUI);

	// 생성 시, 랜덤으로 판매 테이블 로드
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	DataManager->GetItemTradeTable(ItemTradeDatas);

	if (ItemSlotClass == nullptr)
		return;

	ItemSlotInst.SetNum(ItemTradeDatas.Num());
	for (uint8 i = 0; i < ItemTradeDatas.Num(); i++)
	{
		TObjectPtr<UUWItemTradeSlot> SlotInst = CreateWidget<UUWItemTradeSlot>(this, ItemSlotClass);
		SlotInst->Init(i);
		SlotInst->SetSize(SlotSize);
		SlotInst->SetSlotUsingID(ItemTradeDatas[i]->ItemID, ItemTradeDatas[i]->Amount);
		SlotInst->SetPrice(ItemTradeDatas[i]->Price);
		SlotInst->MarkSelected(false);
		SlotInst->OnSlotClicked.BindUObject(this, &UUWItemTrade::ClickSlot);

		ItemSlotInst[i] = SlotInst;
		ItemContainer->AddChild(SlotInst);
	}
}

void UUWItemTrade::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);
	Init();
}

void UUWItemTrade::Init()
{
	SelectedItemDetail->SetVisibility(ESlateVisibility::Hidden);

	ItemSlotInst[SelectedIndex]->MarkSelected(false);
}

void UUWItemTrade::ClickSlot(uint8 _idx)
{
	ItemSlotInst[SelectedIndex]->MarkSelected(false);
	SelectedIndex = _idx;
	ItemSlotInst[SelectedIndex]->MarkSelected(true);

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();

	SelectedItemDetail->SetDetail(DataManager->GetItemData(ItemTradeDatas[SelectedIndex]->ItemID));
	SelectedItemDetail->SetVisibility(ESlateVisibility::Visible);
}
