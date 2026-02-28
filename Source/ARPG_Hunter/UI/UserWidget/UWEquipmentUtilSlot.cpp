// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWEquipmentUtilSlot.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

#include "Data/ItemData.h"

void UUWEquipmentUtilSlot::SetSlot(const FItemData* _itemData, const TArray<FText>* _addictiveText)
{
	Thumbnail->SetBrushFromSoftTexture(_itemData->Item->Thumbnail);
	NameLabel->SetText(FText::FromString(_itemData->Item->Name));
}

void UUWProductSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotButton->OnClicked.AddDynamic(this, &UUWProductSlot::ClickSlot);
}

void UUWProductSlot::ClickSlot()
{
	OnProductSlotClicked.ExecuteIfBound(Index);
}

void UUWIngredientSlot::SetSlot(const FItemData* _itemData, const TArray<FText>* _addictiveText)
{
	Super::SetSlot(_itemData, _addictiveText);

	if (_addictiveText)
		AmountLabel->SetText((*_addictiveText)[0]);
}
