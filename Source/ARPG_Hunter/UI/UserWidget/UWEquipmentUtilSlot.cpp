// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWEquipmentUtilSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "Data/ItemData.h"
#include "Item/Item.h"

void UUWEquipmentUtilSlot::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (AssignedThumbnail)
		Thumbnail->SetBrushFromTexture(AssignedThumbnail);

	if (AssignedName.IsEmpty() == false)
		NameLabel->SetText(AssignedName);
}

void UUWEquipmentUtilSlot::SetSlot(const TWeakObjectPtr<UItem> _item, const TArray<FText>* _addictiveText)
{
	if (_item.IsValid() == false)
		return;

	Thumbnail->SetBrushFromSoftTexture(_item->GetConfig()->Thumbnail);
	Thumbnail->SetVisibility(ESlateVisibility::Visible);
	
	FString ItemName;
	_item->GetItemName(ItemName);
	NameLabel->SetText(FText::FromString(ItemName));
	NameLabel->SetVisibility(ESlateVisibility::Visible);
}

void UUWEquipmentUtilSlot::SetSlot(const FItemData* _itemData, const TArray<FText>* _addictiveText)
{
	Thumbnail->SetBrushFromSoftTexture(_itemData->Item->Thumbnail);
	Thumbnail->SetVisibility(ESlateVisibility::Visible);

	NameLabel->SetText(FText::FromString(_itemData->Item->Name));
	NameLabel->SetVisibility(ESlateVisibility::Visible);
}

void UUWEquipmentUtilSlot::Clear()
{
	Thumbnail->SetBrushFromSoftTexture(nullptr);
	Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	NameLabel->SetVisibility(ESlateVisibility::Hidden);
}

void UUWListElementSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotButton->OnClicked.AddDynamic(this, &UUWListElementSlot::ClickSlot);
}

void UUWListElementSlot::ClickSlot()
{
	OnSlotClicked.ExecuteIfBound(Index);
}

void UUWIngredientSlot::SetSlot(const FItemData* _itemData, const TArray<FText>* _addictiveText)
{
	Super::SetSlot(_itemData, _addictiveText);

	if (_addictiveText && (*_addictiveText).Num() > 0)
		AmountLabel->SetText((*_addictiveText)[0]);
}

void UUWIngredientSlot::SetAmountLabel(const FText& _text, bool _bIsSufficient)
{
	AmountLabel->SetColorAndOpacity(FSlateColor(_bIsSufficient ? ColorOnSufficient : ColorOnInsufficient));
	AmountLabel->SetText(_text);
}