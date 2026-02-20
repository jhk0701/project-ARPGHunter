// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemSlot.h"
#include "Components/Image.h"
#include "Blueprint/DragDropOperation.h"

#include "Define/Enum.h"
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
		OnSlotClicked.ExecuteIfBound(GetIndex());
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

void UUWItemSlot::SetItem(TWeakObjectPtr<UItem> _item)
{
	Super::SetItem(_item);

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
	Super::ClearSlot();

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
