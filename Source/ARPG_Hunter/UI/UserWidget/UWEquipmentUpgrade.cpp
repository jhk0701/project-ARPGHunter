// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWEquipmentUpgrade.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Border.h"
#include "Components/Button.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "Item/Item.h"

#include "UI/ContentWidget/UWCheckBox.h"
#include "UI/UserWidget/UWEquipmentUtilSlot.h"


void UUWEquipmentUpgrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWEquipmentUpgrade::HideUI);

	if (EquipmentSlotClass) 
	{
		EquipmentSlotInst.SetNum(EquipmentSlotInitCount);
		for (uint8 i = 0; i < EquipmentSlotInitCount; ++i)
		{
			TObjectPtr<UUWListElementSlot> SlotInst = CreateWidget<UUWListElementSlot>(GetWorld(), EquipmentSlotClass);
			
			SlotInst->SetIndex(i);
			EquipmentSlotInst[i] = SlotInst;
			EquipmentSlotContainer->AddChild(SlotInst);
			SlotInst->SetVisibility(ESlateVisibility::Collapsed);
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
	TObjectPtr<UInventory> Inventory = GetGameInstance()->GetSubsystem<UPlayerManager>()->GetInventory();
	
	uint8 SlotIdx = 0;
	const TArray<TObjectPtr<UItem>>& Weapons = Inventory->GetContainer(EItemType::WEAPON);
	for (uint8 i = 0; i < Weapons.Num(); ++i)
	{
		if (Weapons[i])
		{
			EquipmentSlotInst[SlotIdx]->SetSlot(Weapons[i]);
			EquipmentSlotInst[SlotIdx]->SetVisibility(ESlateVisibility::Visible);
			SlotIdx++;
		}
	}
	
	const TArray<TObjectPtr<UItem>>& Armors = Inventory->GetContainer(EItemType::ARMOR);
	for (uint8 i = 0; i < Armors.Num(); ++i)
	{
		if (Armors[i])
		{
			EquipmentSlotInst[SlotIdx]->SetSlot(Armors[i]);
			EquipmentSlotInst[SlotIdx]->SetVisibility(ESlateVisibility::Visible);
			SlotIdx++;
		}
	}

	Result->SetVisibility(ESlateVisibility::Hidden);
}
