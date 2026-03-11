// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemTrade.h"
#include "Components/Button.h"
#include "Components/EditableText.h"

#include "Core/Subsystem/PlayerManager.h"
#include "Core/Subsystem/DataManager.h"
#include "Data/ItemTradeData.h"

#include "UI/UserWidget/UWItemDetail.h"
#include "UI/UserWidget/UWItemSlot.h"
#include "UI/UserWidget/UWEquipmentUtilSlot.h"


void UUWItemTrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWItemTrade::HideUI);
	PurchaseButton->OnClicked.AddDynamic(this, &UUWItemTrade::Purchase);
	AmountInputField->OnTextChanged.AddDynamic(this, &UUWItemTrade::HandleTextEditEvent);
	SubTenButton->OnClicked.AddDynamic(this, &UUWItemTrade::ClickSubTen);
	SubOneButton->OnClicked.AddDynamic(this, &UUWItemTrade::ClickSubOne);
	AddOneButton->OnClicked.AddDynamic(this, &UUWItemTrade::ClickAddOne);
	AddTenButton->OnClicked.AddDynamic(this, &UUWItemTrade::ClickAddTen);

	// 생성 시, 랜덤으로 판매 테이블 로드
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	DataManager->GetItemTradeTable(ItemTradeDatas);

	if (ItemSlotClass == nullptr)
		return;

	RemainAmount.SetNum(ItemTradeDatas.Num());
	ItemSlotInst.SetNum(ItemTradeDatas.Num());
	for (uint8 i = 0; i < ItemTradeDatas.Num(); i++)
	{
		RemainAmount[i] = ItemTradeDatas[i]->Amount;

		TObjectPtr<UUWItemTradeSlot> SlotInst = CreateWidget<UUWItemTradeSlot>(this, ItemSlotClass);
		SlotInst->Init(i);
		SlotInst->SetSize(SlotSize);
		SlotInst->SetSlotUsingID(ItemTradeDatas[i]->ItemID, ItemTradeDatas[i]->Amount);
		SlotInst->SetPrice(ItemTradeDatas[i]->Price);
		SlotInst->MarkSelected(false);
		SlotInst->OnSlotClicked.BindUObject(this, &UUWItemTrade::ClickSlot);
		// SlotInst->OnSlotHovered.BindUObject(this, &UUWItemTrade::ClickSlot);

		ItemSlotInst[i] = SlotInst;
		ItemContainer->AddChild(SlotInst);
	}
}

void UUWItemTrade::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
	Init();
}

void UUWItemTrade::Init()
{
	ClickSlot(0);
	/*SelectedItemDetail->SetVisibility(ESlateVisibility::Hidden);
	ItemSlotInst[SelectedIndex]->MarkSelected(false);

	Option->SetVisibility(ESlateVisibility::Hidden);

	InitOption();*/
}

void UUWItemTrade::ClickSlot(uint8 _idx)
{
	ItemSlotInst[SelectedIndex]->MarkSelected(false);
	SelectedIndex = _idx;
	ItemSlotInst[SelectedIndex]->MarkSelected(true);

	Option->SetVisibility(ESlateVisibility::Visible);

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();

	SelectedItemDetail->SetDetail(DataManager->GetItemData(ItemTradeDatas[SelectedIndex]->ItemID));
	SelectedItemDetail->SetVisibility(ESlateVisibility::Visible);

	if(RemainAmount[SelectedIndex] > 0)
		InitOption();
	else // 품절
		Option->SetVisibility(ESlateVisibility::Hidden);
}

void UUWItemTrade::InitOption()
{
	InputAmount = 1;
	SetTextWithoutEvent(FText::AsNumber(InputAmount));
	UpdatePriceSlot();
}

void UUWItemTrade::UpdatePriceSlot()
{
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	uint32 PlayerGold = PlayerManager->GetGold();

	FText GoldFormat = FText::FromString(TEXT("{0} / {1} G"));
	uint32 TotalPrice = ItemTradeDatas[SelectedIndex]->Price * InputAmount;

	bool bIsSufficient = PlayerGold >= TotalPrice;
	PriceSlot->SetAmountLabel(FText::Format(GoldFormat, PlayerGold, TotalPrice), bIsSufficient);
	PurchaseButton->SetIsEnabled(bIsSufficient);
}

void UUWItemTrade::HandleTextEditEvent(const FText& Text)
{
	FString InputStr = Text.ToString();
	if (InputStr.IsNumeric())
	{
		int32 InNum = FMath::Max(FCString::Atoi(*InputStr), 1);
		SetAmount(static_cast<uint32>(InNum));
	}
}

void UUWItemTrade::ClickSubOne()
{
	ChangeAmount(-1);
}

void UUWItemTrade::ClickSubTen()
{
	ChangeAmount(-10);
}

void UUWItemTrade::ClickAddOne()
{
	ChangeAmount(1);
}

void UUWItemTrade::ClickAddTen()
{
	ChangeAmount(10);
}

void UUWItemTrade::ChangeAmount(int32 _amount)
{
	if (_amount < 0 && InputAmount <= static_cast<uint32>(FMath::Abs(_amount)))
		InputAmount = 1;
	else
		InputAmount += _amount;

	SetAmount(InputAmount);
}

void UUWItemTrade::SetAmount(uint32 _amount)
{
	InputAmount = FMath::Min(_amount, RemainAmount[SelectedIndex]);
	UpdatePriceSlot();

	SetTextWithoutEvent(FText::AsNumber(InputAmount));
}

void UUWItemTrade::SetTextWithoutEvent(const FText& _text)
{
	UEditableText::FOnEditableTextChangedEvent ChangeTmp = AmountInputField->OnTextChanged;
	AmountInputField->OnTextChanged.Clear();
	UEditableText::FOnEditableTextCommittedEvent CommittedTmp = AmountInputField->OnTextCommitted;
	AmountInputField->OnTextCommitted.Clear();

	AmountInputField->SetText(_text);

	AmountInputField->OnTextChanged = ChangeTmp;
	AmountInputField->OnTextCommitted = CommittedTmp;
}

void UUWItemTrade::Purchase()
{
	uint32 PurchaseAmount = FMath::Min(InputAmount, RemainAmount[SelectedIndex]);
	
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

	// 비용 지불
	if (PlayerManager->TrySubGold(PurchaseAmount * ItemTradeDatas[SelectedIndex]->Price) == false)
		return;

	// 아이템 전달
	TWeakObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
	PlayerManager->AddItem(ItemTradeDatas[SelectedIndex]->ItemID, PurchaseAmount);

	// 선택한 아이템 UI 변경
	RemainAmount[SelectedIndex] -= PurchaseAmount;
	ItemSlotInst[SelectedIndex]->SetAmount(RemainAmount[SelectedIndex]);
	ClickSlot(SelectedIndex);
}