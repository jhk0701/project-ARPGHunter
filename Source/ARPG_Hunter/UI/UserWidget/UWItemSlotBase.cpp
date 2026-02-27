// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemSlotBase.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Define/Enum.h"
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

void UUWItemSlotBase::SetItem(TWeakObjectPtr<UItem> _item)
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
