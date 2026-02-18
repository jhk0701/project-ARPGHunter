// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInventory.h"
#include "Components/WrapBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Define/Enum.h"
#include "Data/ItemData.h"
#include "Item/Item.h"
#include "UI/ContentWidget/UWCheckBox.h"
#include "UI/UserWidget/UWItemSlot.h"
#include "UI/UserWidget/UWItemDetail.h"

void UUWInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	CurType = EItemType::WEAPON;

	CloseButton->OnClicked.AddDynamic(this, &UUWInventory::ClickCloseButton);

	for (uint8 i = 0; i < CategoryContainer->GetChildrenCount(); ++i)
	{
		TObjectPtr<UUWCheckBox> CheckBox = Cast<UUWCheckBox>(CategoryContainer->GetChildAt(i));
		CheckBox->Init();
		CheckBox->OnCheckBoxChanged.AddUObject(this, &UUWInventory::ClickCategoryCheckBox);

		EItemType Type = static_cast<EItemType>(CheckBox->GetOption());
		Category.Add(Type, CheckBox);
	}
}

void UUWInventory::ShowUI()
{
	Super::ShowUI();

	SelectedItemDetail->SetVisibility(ESlateVisibility::Hidden);
	UpdateSlot();
}

void UUWInventory::Init(uint8 _initSize, uint32 _gold, TFunction<const TArray<TObjectPtr<UItem>>*(EItemType)> _getItemArrFunc)
{
	GetItemArrFunc = _getItemArrFunc;

	check(ItemSlotClass); // 없는 경우 크래시

	ItemSlots.SetNum(_initSize);
	for (int i = 0; i < _initSize; ++i)
	{
		ItemSlots[i] = CreateWidget<UUWItemSlot>(this, ItemSlotClass);
		ItemSlots[i]->Init(i, SlotSize);
		ItemSlots[i]->OnSlotClicked.BindUObject(this, &UUWInventory::OnSlotClicked);

		SlotContainer->AddChild(ItemSlots[i]);
	}

	SetGoldLabel(_gold);
}

void UUWInventory::SetSlot(uint8 _idx, TObjectPtr<UItem> _item)
{
	ItemSlots[_idx]->SetItem(_item);
}

void UUWInventory::SetGoldLabel(uint32 _goldValue)
{
	GoldLabel->SetText(FText::FromString(FString::FormatAsNumber(_goldValue).Append(TEXT(" G"))));
}

void UUWInventory::ClickCloseButton()
{
	HideUI();
}

void UUWInventory::ClickCategoryCheckBox(bool _bIsChecked, uint8 _opt)
{
	CurType = static_cast<EItemType>(_opt);
	
	for(const TPair<EItemType, TObjectPtr<UUWCheckBox>>& Pair : Category)
	{
		if (Pair.Key == CurType)
			continue;

		Pair.Value->UpdateStateWithoutEvent(false);
	}

	SelectedItemDetail->SetVisibility(ESlateVisibility::Hidden);
	UpdateSlot();
}

void UUWInventory::UpdateSlot()
{
	if (GetItemArrFunc == nullptr)
		return;

	const TArray<TObjectPtr<UItem>>* ItemArr = GetItemArrFunc(CurType);
	for (uint8 i = 0; i < ItemArr->Num(); ++i)
		SetSlot(i, (*ItemArr)[i]);
}

void UUWInventory::OnSlotClicked(uint8 _index)
{
	if (GetItemArrFunc == nullptr)
		return;

	const TArray<TObjectPtr<UItem>>* ItemArr = GetItemArrFunc(CurType);
	SelectedItemDetail->SetDetail((*ItemArr)[_index]);

	SelectedItemDetail->SetVisibility(ESlateVisibility::Visible);
}
