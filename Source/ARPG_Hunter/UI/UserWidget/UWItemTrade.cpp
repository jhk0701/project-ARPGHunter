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
		TObjectPtr<UUWItemSlotIndicate> SlotInst = CreateWidget<UUWItemSlotIndicate>(this, ItemSlotClass);
		SlotInst->SetSlotUsingID(ItemTradeDatas[i]->ItemID, ItemTradeDatas[i]->Amount);
		SlotInst->SetSize(SlotSize);
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
}