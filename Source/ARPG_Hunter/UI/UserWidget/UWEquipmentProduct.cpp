// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWEquipmentProduct.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"

void UUWProductSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Button->OnClicked.AddDynamic(this, &UUWProductSlot::ClickSlot);
}

void UUWProductSlot::Init(uint8 _idx)
{
	Index = _idx;
}

void UUWProductSlot::SetSlot(const FText& _nameText, TObjectPtr<UTexture2D> _thumbnail)
{
	NameLabel->SetText(_nameText);
	Thumbnail->SetBrushFromTexture(_thumbnail);
}

void UUWProductSlot::ClickSlot()
{
	OnProductSlotClicked.ExecuteIfBound(Index);
}
