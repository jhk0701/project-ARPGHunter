// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UserWidget/UWEquipmentUpgrade.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "Item/Item.h"
#include "Data/ItemData.h"

#include "UI/UserWidget/UWCategory.h"
#include "UI/UserWidget/UWEquipmentUtilSlot.h"


void UUWEquipmentUpgrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Inventory = GetGameInstance()->GetSubsystem<UPlayerManager>()->GetInventory();

	CloseButton->OnClicked.AddDynamic(this, &UUWEquipmentUpgrade::HideUI);
	ItemCategory->OnSelected.AddUObject(this, &UUWEquipmentUpgrade::SelectCategory);
	
	if (EquipmentSlotClass) 
	{
		uint8 Size = Inventory->GetContainerSize();
		
		EquipmentSlotInst.SetNum(Size);
		for (uint8 i = 0; i < Size; ++i)
		{
			TObjectPtr<UUWListElementSlot> SlotInst = CreateWidget<UUWListElementSlot>(GetWorld(), EquipmentSlotClass);
			SlotInst->SetIndex(i);
			SlotInst->OnSlotClicked.BindUObject(this, &UUWEquipmentUpgrade::SelectSlot);
			SlotInst->SetVisibility(ESlateVisibility::Collapsed);

			EquipmentSlotInst[i] = SlotInst;
			EquipmentSlotContainer->AddChild(SlotInst);
		}
	}
}

void UUWEquipmentUpgrade::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);
	Init();
}

void UUWEquipmentUpgrade::Init()
{
	// 플레이어 장비 출력
	SelectCategory(static_cast<uint32>(EItemType::WEAPON));

	Result->SetVisibility(ESlateVisibility::Hidden);
}

void UUWEquipmentUpgrade::SelectCategory(uint8 _option)
{
	if (Inventory.IsValid() == false)
		return;

	CurItemType = static_cast<EItemType>(_option);

	const TArray<TObjectPtr<UItem>>& Items = Inventory->GetContainer(CurItemType);

	for (uint8 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i])
		{
			EquipmentSlotInst[i]->SetSlot(Items[i]);
			EquipmentSlotInst[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
			EquipmentSlotInst[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUWEquipmentUpgrade::SelectSlot(uint8 _index)
{
	if (Inventory.IsValid() == false)
		return;

	const TArray<TObjectPtr<UItem>>& Items = Inventory->GetContainer(CurItemType);
	TObjectPtr<UItemConfig> Config = Items[_index]->GetConfig();
	ItemThumbnail->SetBrushFromTexture(Config->Thumbnail);
	ItemNameLabel->SetText(FText::FromString(Config->Name));

	TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(Items[_index]);

	// TODO : 최대 업그레이드인지 확인
	CurGradeLabel->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Equipment->GetGrade())));
	NextGradeLabel->SetText(FText::FromString(FString::Printf(TEXT("+%d"), Equipment->GetGrade() + 1)));
}
