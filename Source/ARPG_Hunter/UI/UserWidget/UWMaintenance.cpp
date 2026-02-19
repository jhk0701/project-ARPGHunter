// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWMaintenance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/WrapBox.h"

#include "Define/Enum.h"
#include "Item/Item.h"
#include "UI/UserWidget/UWItemSlot.h"
#include "UI/UserWidget/UWStatInfo.h"

void UUWMaintenance::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 생성 시, 초기화
	CloseButton->OnClicked.AddDynamic(this, &UUWMaintenance::ClickCloseButton);

	if (WeaponContainer->GetChildrenCount() > 1) 
	{
		TObjectPtr<UUWItemSlot> WeaponSlot = Cast<UUWItemSlot>(WeaponContainer->GetChildAt(1));
		MapEquipmentSlot.Add(EEquipmentType::WEAPON, WeaponSlot);

		WeaponSlot->Init(0);
		WeaponSlot->OnSlotClicked.BindLambda([this](uint8 _opt) { ClickEquipmentSlot(EItemType::WEAPON, _opt); });
	}

	if (ArmorContainer->GetChildrenCount() > 3) 
	{
		for (uint8 i = static_cast<uint8>(EEquipmentType::HEAD); i < static_cast<uint8>(EEquipmentType::END); ++i)
		{
			EEquipmentType Type = static_cast<EEquipmentType>(i);
			TObjectPtr<UUWItemSlot> ArmorSlot = Cast<UUWItemSlot>(ArmorContainer->GetChildAt(i));
			MapEquipmentSlot.Add(Type, ArmorSlot);

			ArmorSlot->Init(i);
			ArmorSlot->OnSlotClicked.BindLambda([this](uint8 _opt) { ClickEquipmentSlot(EItemType::ARMOR, _opt); });
		}
	}

	if (QuickSlotContainer) 
	{
		int32 Cnt = QuickSlotContainer->GetChildrenCount();
		ArrQuickSlot.SetNum(Cnt);

		for (int32 i = 0; i < Cnt; ++i)
		{
			TObjectPtr<UUWItemSlot> QuickSlot = Cast<UUWItemSlot>(QuickSlotContainer->GetChildAt(i));
			ArrQuickSlot[i] = QuickSlot;

			QuickSlot->Init(i);
			QuickSlot->OnSlotClicked.BindUObject(this, &UUWMaintenance::ClickQuickSlot);
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

void UUWMaintenance::Init(const FUWMaintenanceInitParam& _param)
{
	for (const TPair<ECharacterStatType, uint32>& Pair : _param.PlayerStat)
		MapStatInfo[Pair.Key]->SetStatValue(Pair.Value, _param.EquipmentStat[Pair.Key]);

	for (const TPair<EEquipmentType, TObjectPtr<UEquipmentItem>>& Pair : _param.Equipment)
		MapEquipmentSlot[Pair.Key]->SetItem(Pair.Value);

	for (uint8 i = 0; i < _param.QuickSlot.Num(); ++i)
		ArrQuickSlot[i]->SetItem(_param.QuickSlot[i]);
}

void UUWMaintenance::SetStatInfo(const TMap<ECharacterStatType, uint32>& _playerStat, const TMap<ECharacterStatType, uint32>& _equipmentStat)
{
	for (const TPair<ECharacterStatType, uint32>& Pair : _playerStat)
		MapStatInfo[Pair.Key]->SetStatValue(Pair.Value, _equipmentStat[Pair.Key]);
}

void UUWMaintenance::SetEquipment(EEquipmentType _type, TObjectPtr<UEquipmentItem> _equipment)
{
	MapEquipmentSlot[_type]->SetItem(_equipment);
}

void UUWMaintenance::SetQuickSlot(uint8 _index, TObjectPtr<UConsumableItem> _consumable)
{
	ArrQuickSlot[_index]->SetItem(_consumable);
}

void UUWMaintenance::ClickCloseButton()
{
	HideUI();
}

void UUWMaintenance::ClickEquipmentSlot(EItemType _type, uint8 _opt)
{
	OnEquipmentSlotClicked.ExecuteIfBound(_type, _opt);
}

void UUWMaintenance::ClickQuickSlot(uint8 _idx)
{
	OnQuickSlotClicked.ExecuteIfBound(_idx);
}
