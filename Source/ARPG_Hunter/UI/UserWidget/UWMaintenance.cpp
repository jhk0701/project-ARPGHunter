// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWMaintenance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"

#include "Define/Enum.h"
#include "Item/Item.h"
#include "UI/UserWidget/UWItemSlot.h"

void UUWStatInfo::SetStatName(ECharacterStatType _type)
{
	NameLabel->SetText(FText::FromName(EnumToName(_type)));
}

void UUWStatInfo::SetStatValue(uint32 _value)
{
	ValueLabel->SetText(FText::AsNumber(_value));
}

void UUWMaintenance::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWMaintenance::ClickCloseButton);

	if (WeaponContainer->GetChildrenCount() > 1) 
	{
		TObjectPtr<UUWItemSlot> WeaponSlot = Cast<UUWItemSlot>(WeaponContainer->GetChildAt(1));
		EquipmentSlot.Add(EEquipmentType::WEAPON, WeaponSlot);
	}

	if (ArmorContainer->GetChildrenCount() > 3) 
	{
		for (uint8 i = static_cast<uint8>(EEquipmentType::HEAD); i < static_cast<uint8>(EEquipmentType::END); ++i)
		{
			EEquipmentType Type = static_cast<EEquipmentType>(i);
			TObjectPtr<UUWItemSlot> ArmorSlot = Cast<UUWItemSlot>(ArmorContainer->GetChildAt(i));

			EquipmentSlot.Add(Type, ArmorSlot);
		}
	}
}

void UUWMaintenance::Init(const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& _equipment)
{
	for (const TPair<EEquipmentType, TObjectPtr<UEquipmentItem>>& Pair : _equipment)
		EquipmentSlot[Pair.Key]->SetItem(Pair.Value);
}

void UUWMaintenance::SetEquipment(EEquipmentType _type, TObjectPtr<UEquipmentItem> _equipment)
{
	EquipmentSlot[_type]->SetItem(_equipment);
}

void UUWMaintenance::ClickCloseButton()
{
	HideUI();
}
