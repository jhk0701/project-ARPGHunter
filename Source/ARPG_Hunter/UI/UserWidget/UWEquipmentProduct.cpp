// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWEquipmentProduct.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/Border.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager.h"
#include "Data/ItemData.h"
#include "Data/ItemProductData.h"

void UUWProductSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotButton->OnClicked.AddDynamic(this, &UUWProductSlot::ClickSlot);
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

void UUWIngredientSlot::SetSlot(const FText& _nameText, const FText& _amountText, TObjectPtr<UTexture2D> _thumbnail)
{
	NameLabel->SetText(_nameText);
	AmountLabel->SetText(_amountText);
	Thumbnail->SetBrushFromTexture(_thumbnail);
}


void UUWEquipmentProduct::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWEquipmentProduct::HideUI);

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	DataManager->GetAllItemProduct(DataArray);

	if (ProductSlotClass)
	{
		ProductSlotInst.SetNum(DataArray.Num());
		for (uint8 i = 0; i < DataArray.Num(); ++i)
		{
			FItemData* ItemData = DataManager->GetItemData(DataArray[i]->ItemID);
			TObjectPtr<UUWProductSlot> SlotInst = CreateWidget<UUWProductSlot>(GetWorld(), ProductSlotClass);

			SlotInst->SetIndex(i);
			SlotInst->SetSlot(FText::FromString(ItemData->Item->Name), ItemData->Item->Thumbnail);
			SlotInst->OnProductSlotClicked.BindUObject(this, &UUWEquipmentProduct::ClickProductSlot);

			ProductSlotInst[i] = SlotInst;
			ProductSlotContainer->AddChild(SlotInst);
		}
	}
}

void UUWEquipmentProduct::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);

	Init();
}

void UUWEquipmentProduct::Init()
{
	CurIndex = 0;
	IngredientDetail->SetVisibility(ESlateVisibility::Hidden);
}

void UUWEquipmentProduct::ClickProductSlot(uint8 _index)
{
	CurIndex = _index;

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FItemData* ItemData = DataManager->GetItemData(DataArray[CurIndex]->ItemID);

	ItemNameLabel->SetText(FText::FromString(ItemData->Item->Name)); // ItemData->Item->
	// ItemNameLabel->SetText(FText::FromString(EnumToString(ItemData->Type)));

	IngredientDetail->SetVisibility(ESlateVisibility::Visible);
}
