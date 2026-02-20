// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemSlot.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/DragDropOperation.h"

#include "Define/Enum.h"
#include "Data/ItemData.h"
#include "Item/Item.h"

void UUWItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	MarkSelected(false);
	MarkEquipped(false);
}

FReply UUWItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnSlotClicked.ExecuteIfBound(Index);
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UUWItemSlot::Init(uint8 _idx)
{
	Index = _idx;
}

void UUWItemSlot::Init(uint8 _idx, const FVector2D& _size)
{
	Index = _idx;
	SetSize(_size);
}

void UUWItemSlot::SetSize(const FVector2D& _size)
{
	Frame->SetWidthOverride(_size.X);
	Frame->SetHeightOverride(_size.Y);
}

void UUWItemSlot::SetItem(TWeakObjectPtr<UItem> _item)
{
	if (_item.IsValid() == false)
	{
		ClearSlot();
		return;
	}

	Thumbnail->SetBrushFromTexture(_item->GetConfig()->Thumbnail);
	Thumbnail->SetVisibility(ESlateVisibility::Visible);

	if (_item->GetAmount() > 1)
	{
		AmountLabel->SetVisibility(ESlateVisibility::Visible);
		AmountLabel->SetText(FText::AsNumber(_item->GetAmount()));
	}
	else
		AmountLabel->SetVisibility(ESlateVisibility::Hidden);

	bool bIsEquiped = false;
	if (_item->GetType() >= EItemType::CONSUMABLE) 
	{
		if (TObjectPtr<UConsumableItem> Consumable = Cast<UConsumableItem>(_item))
			bIsEquiped = Consumable->GetQuickSlotIndex() >= 0;
		else if (TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(_item))
			bIsEquiped = Equipment->GetEquipmentIndex() >= 0;
	}

	MarkEquipped(bIsEquiped);
}

void UUWItemSlot::ClearSlot()
{
	Thumbnail->SetBrushFromTexture(nullptr);
	Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	AmountLabel->SetVisibility(ESlateVisibility::Hidden);
	MarkEquipped(false);
}

void UUWItemSlot::MarkSelected(bool _bIsSelected)
{
	SelectedMark->SetVisibility(_bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UUWItemSlot::MarkEquipped(bool _bIsEquipped)
{
	EquippedMark->SetVisibility(_bIsEquipped ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UUWItemSlot::IsEquipped() const
{
	return EquippedMark->GetVisibility() == ESlateVisibility::Visible;
}
