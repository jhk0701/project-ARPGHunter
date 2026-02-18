// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemSlot.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Blueprint/DragDropOperation.h"

#include "Data/ItemData.h"
#include "Item/Item.h"

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

void UUWItemSlot::SetItem(TObjectPtr<UItem> _item)
{
	if (nullptr == _item)
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

void UUWItemSlot::ClearSlot()
{
	Thumbnail->SetBrushFromTexture(nullptr);
	Thumbnail->SetVisibility(ESlateVisibility::Hidden);
	AmountLabel->SetVisibility(ESlateVisibility::Hidden);
}
