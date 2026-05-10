// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager/DataManager.h"
#include "Data/ItemData.h"
#include "Item/Item.h"


void UUWItemSlotBase::Init(uint8 _idx)
{
	Index = _idx;
}

void UUWItemSlotBase::SetSize(const FVector2D& _size)
{
	Frame->SetWidthOverride(_size.X);
	Frame->SetHeightOverride(_size.Y);
}

void UUWItemSlotBase::SetAmount(uint32 _amount)
{
	AmountLabel->SetText(FText::AsNumber(_amount));
}

void UUWItemSlotBase::SetSlot(TWeakObjectPtr<UItem> _item)
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
}

void UUWItemSlotBase::SetSlotUsingID(const FName& _id, uint32 _amount)
{
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FItemData* ItemData = DataManager->GetItemData(_id);
	if (ItemData == nullptr)
		return;

	Thumbnail->SetBrushFromTexture(ItemData->Item->Thumbnail);
	Thumbnail->SetVisibility(ESlateVisibility::Visible);

	if (_amount > 1)
	{
		AmountLabel->SetVisibility(ESlateVisibility::Visible);
		AmountLabel->SetText(FText::AsNumber(_amount));
	}
	else
		AmountLabel->SetVisibility(ESlateVisibility::Hidden);
}

void UUWItemSlotBase::SetThumbnailOnly(TObjectPtr<UTexture2D> _tex)
{
	Thumbnail->SetBrushFromTexture(_tex);
	Thumbnail->SetVisibility(ESlateVisibility::Visible);

	AmountLabel->SetVisibility(ESlateVisibility::Hidden);
}

void UUWItemSlotBase::ClearSlot()
{
	Thumbnail->SetBrushFromTexture(nullptr);
	Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	AmountLabel->SetVisibility(ESlateVisibility::Hidden);
}

void UUWSelectableItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MarkSelected(false);
}

void UUWSelectableItemSlot::MarkSelected(bool _bIsSelected)
{
	SelectedMark->SetVisibility(_bIsSelected ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

FReply UUWSelectableItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		OnSlotClicked.ExecuteIfBound(GetIndex());

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UUWSelectableItemSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnSlotHovered.ExecuteIfBound(GetIndex());
}

void UUWItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	MarkEquipped(false);
}

void UUWItemSlot::SetSlot(TWeakObjectPtr<UItem> _item)
{
	Super::SetSlot(_item);

	if (_item.IsValid() == false)
		return;

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

void UUWItemSlot::SetThumbnailOnly(TObjectPtr<UTexture2D> _tex)
{
	Super::SetThumbnailOnly(_tex);

	MarkSelected(false);
	MarkEquipped(false);
}

void UUWItemSlot::ClearSlot()
{
	Super::ClearSlot();

	MarkEquipped(false);
}


void UUWItemSlot::MarkEquipped(bool _bIsEquipped)
{
	EquippedMark->SetVisibility(_bIsEquipped ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UUWItemSlot::IsEquipped() const
{
	return EquippedMark->GetVisibility() == ESlateVisibility::Visible;
}

void UUWItemSlotIndicate::SetMessage(const FText& _content)
{
	MessageLabel->SetText(_content);
}

void UUWItemTradeSlot::SetPrice(uint32 _price)
{
	PriceLabel->SetText(FText::Format(FText::FromString(TEXT("{0} G")), _price));
}
