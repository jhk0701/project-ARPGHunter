// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemSlot.h"
#include "Components/SizeBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Data/ItemData.h"
#include "Item/Item.h"

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
	AmountLabel->SetText(FText::AsNumber(_item->GetAmount()));
}

void UUWItemSlot::ClearSlot()
{
	Thumbnail->SetBrushFromTexture(nullptr);
	AmountLabel->SetText(FText::GetEmpty());
}
