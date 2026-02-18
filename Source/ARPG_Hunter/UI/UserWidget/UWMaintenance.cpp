// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWMaintenance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"

#include "Define/Enum.h"
#include "Item/Item.h"
#include "UI/UserWidget/UWItemSlot.h"

void UUWStatInfo::SetStatName(ECharacterStatType _type)
{
	NameLabel->SetText(FText::FromName(EnumToName(_type)));
}

void UUWStatInfo::SetStatValue(uint32 _baseValue, uint32 _equipmentValue)
{
	FString str = FString::Printf(TEXT("%d + (%d) = %d"), _baseValue, _equipmentValue, (_baseValue + _equipmentValue));
	ValueLabel->SetText(FText::FromString(str));
}

void UUWMaintenance::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 생성 시, 초기화

	CloseButton->OnClicked.AddDynamic(this, &UUWMaintenance::ClickCloseButton);

	if (WeaponContainer->GetChildrenCount() > 1) 
	{
		TObjectPtr<UUWItemSlot> WeaponSlot = Cast<UUWItemSlot>(WeaponContainer->GetChildAt(1));
		MapEquipmentSlot.Add(EEquipmentType::WEAPON, WeaponSlot);
	}

	if (ArmorContainer->GetChildrenCount() > 3) 
	{
		for (uint8 i = static_cast<uint8>(EEquipmentType::HEAD); i < static_cast<uint8>(EEquipmentType::END); ++i)
		{
			EEquipmentType Type = static_cast<EEquipmentType>(i);
			TObjectPtr<UUWItemSlot> ArmorSlot = Cast<UUWItemSlot>(ArmorContainer->GetChildAt(i));

			MapEquipmentSlot.Add(Type, ArmorSlot);
		}
	}

	if (StatInfoUIClass) 
	{
		for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
		{
			ECharacterStatType Type = static_cast<ECharacterStatType>(i);
			TObjectPtr<UUWStatInfo> StatInfoInst = CreateWidget<UUWStatInfo>(GetWorld(), StatInfoUIClass);
			
			MapStatInfo.Add(Type, StatInfoInst);
			StatInfoInst->SetStatName(Type);
			StatContainer->AddChild(StatInfoInst);
		}
	}
}

void UUWMaintenance::Init(const TMap<ECharacterStatType, uint32>& _stat, const TMap<ECharacterStatType, uint32>& _equipmentStat, const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& _equipment)
{
	for (const TPair<ECharacterStatType, uint32>& Pair : _stat)
		MapStatInfo[Pair.Key]->SetStatValue(Pair.Value, _equipmentStat[Pair.Key]);

	for (const TPair<EEquipmentType, TObjectPtr<UEquipmentItem>>& Pair : _equipment)
		MapEquipmentSlot[Pair.Key]->SetItem(Pair.Value);
}

void UUWMaintenance::SetStatInfo(const TMap<ECharacterStatType, uint32>& _stat, const TMap<ECharacterStatType, uint32>& _equipmentStat)
{
	for (const TPair<ECharacterStatType, uint32>& Pair : _stat)
		MapStatInfo[Pair.Key]->SetStatValue(Pair.Value, _equipmentStat[Pair.Key]);
}

void UUWMaintenance::SetEquipment(EEquipmentType _type, TObjectPtr<UEquipmentItem> _equipment)
{
	MapEquipmentSlot[_type]->SetItem(_equipment);
}

void UUWMaintenance::ClickCloseButton()
{
	HideUI();
}
