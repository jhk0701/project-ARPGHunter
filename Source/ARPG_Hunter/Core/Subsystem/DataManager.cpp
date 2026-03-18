// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/Subsystem/DataManager.h"
#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"

#include "Define/Enum.h"
#include "Data/WeaponConfig.h"
#include "Data/EquipmentSuccessData.h"
#include "Data/MonsterData.h"
#include "Data/StageData.h"
#include "Data/ItemData.h"
#include "Data/ItemProductData.h"
#include "Data/ItemTradeData.h"
#include "Data/DialogData.h"
#include "Data/EquipmentUpgradeData.h"

#include "Define/Debug.h"

UDataManager::UDataManager()
{
	static ConstructorHelpers::FObjectFinder<UWeaponType> WeaponTypeDataFinder(TEXT("/Script/ARPG_Hunter.WeaponType'/Game/03-Data/WeaponTypeData.WeaponTypeData'"));
	if (WeaponTypeDataFinder.Succeeded())
		WeaponTypeData = WeaponTypeDataFinder.Object;

	static ConstructorHelpers::FObjectFinder<UCurveTable> PlayerLvCurveFinder(TEXT("/Script/Engine.CurveTable'/Game/03-Data/CT_PlayerLevel.CT_PlayerLevel'"));
	if (PlayerLvCurveFinder.Succeeded())
		PlayerLvCurveTable = PlayerLvCurveFinder.Object;

	static ConstructorHelpers::FObjectFinder<UEquipmentSuccessData> SuccessDataFinder(TEXT("/Script/ARPG_Hunter.EquipmentSuccessData'/Game/03-Data/EquipmentSuccessData.EquipmentSuccessData'"));
	if (SuccessDataFinder.Succeeded())
		SuccessData = SuccessDataFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_MonsterData.DT_MonsterData'"));
	if (MonsterDataTableFinder.Succeeded())
		MonsterDataTable = MonsterDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UCurveTable> MonsterLvCurveFinder(TEXT("/Script/Engine.CurveTable'/Game/03-Data/CT_MonsterLevel.CT_MonsterLevel'"));
	if (MonsterLvCurveFinder.Succeeded())
		MonsterLvCurveTable = MonsterLvCurveFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> RegionDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_RegionData.DT_RegionData'"));
	if(RegionDataTableFinder.Succeeded())
		RegionDataTable = RegionDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> StageDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_StageData.DT_StageData'"));
	if (StageDataTableFinder.Succeeded())
		StageDataTable = StageDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_ItemData.DT_ItemData'"));
	if (ItemDataTableFinder.Succeeded())
		ItemDataTable = ItemDataTableFinder.Object;
	
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemProductDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_ItemProductData.DT_ItemProductData'"));
	if (ItemProductDataTableFinder.Succeeded())
		ItemProductDataTable = ItemProductDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UItemTradeConfig> ItemTradeConfigFinder(TEXT("/Script/ARPG_Hunter.ItemTradeConfig'/Game/03-Data/TradeData/TradeConfig.TradeConfig'"));
	if (ItemTradeConfigFinder.Succeeded())
		ItemTradeData = ItemTradeConfigFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DialogDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_DialogData.DT_DialogData'"));
	if (DialogDataTableFinder.Succeeded())
		DialogDataTable = DialogDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> EquipmentUpgradeTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_EquipmentUpgrade.DT_EquipmentUpgrade'"));
	if (EquipmentUpgradeTableFinder.Succeeded())
		EquipmentUpgradeTable = EquipmentUpgradeTableFinder.Object;
}

TObjectPtr<UWeaponConfig> UDataManager::GetWeaponConfig(EWeaponType _type) const
{
	return WeaponTypeData->Map[_type];
}

float UDataManager::GetPlayerLvCurve(uint32 _lv, const FName& _rowName)
{
	FSimpleCurve* Curve = PlayerLvCurveTable->FindSimpleCurve(_rowName, TEXT("Player Lv Curve Search"), false);

	if (Curve == nullptr)
		return -1.0f;

	return Curve->Eval(_lv);
}

FMonsterData* UDataManager::GetMonsterData(const FName& _id) const
{
	return MonsterDataTable->FindRow<FMonsterData>(_id, TEXT("Monster Data Table Search"));
}

float UDataManager::GetMonsterLvCurve(uint32 _lv, const FName& _rowName)
{
	FSimpleCurve* Curve = MonsterLvCurveTable->FindSimpleCurve(_rowName, TEXT("Monster Lv Curve Search"), false);

	if (Curve == nullptr)
		return 0.0f;

	return Curve->Eval(_lv);
}

FRegionData* UDataManager::GetRegionData(const FName& _id) const
{
	return RegionDataTable->FindRow<FRegionData>(_id, TEXT("Region Data Table Search"));
}

FStageData* UDataManager::GetStageData(const FName& _id) const
{
	return StageDataTable->FindRow<FStageData>(_id, TEXT("Stage Data Table Search"));
}

FItemData* UDataManager::GetItemData(const FName& _id) const
{
	return ItemDataTable->FindRow<FItemData>(_id, TEXT("Item Data Table Search"));
}

FItemProductData* UDataManager::GetItemProductData(const FName& _id) const
{
	return ItemProductDataTable->FindRow<FItemProductData>(_id, TEXT("Item Product Data Table Search"));
}

void UDataManager::GetAllItemProduct(TArray<FItemProductData*>& _outArr) const
{
	ItemProductDataTable->GetAllRows(TEXT("Item Product Data Table Search"), _outArr);
}

void UDataManager::GetItemTradeTable(TArray<FItemTradeData*>& _outTradeData) const
{
	ItemTradeData->GetRandomTable()->GetAllRows<FItemTradeData>(TEXT("Item Trade Data Table Search"), _outTradeData);
}

FDialogData* UDataManager::GetDialogData(const FName& _id) const
{
	return DialogDataTable->FindRow<FDialogData>(_id, TEXT("Dialog Data Table Search"));
}

FEquipmentUpgradeData* UDataManager::GetUpgradeData(uint8 _rank, uint8 _grade, EEquipmentType _type)
{
	FName ID = FEquipmentUpgradeData::GetUpgradeID(_rank, _grade, _type);
	return EquipmentUpgradeTable->FindRow<FEquipmentUpgradeData>(ID, TEXT("Upgrade Data Table Search"));
}