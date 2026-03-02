// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystem/DataManager.h"
#include "Engine/DataTable.h"

#include "Define/Enum.h"
#include "Data/WeaponConfig.h"
#include "Data/MonsterData.h"
#include "Data/StageData.h"
#include "Data/ItemData.h"
#include "Data/ItemProductData.h"
#include "Data/DialogData.h"
#include "Data/EquipmentUpgradeData.h"

UDataManager::UDataManager()
{
	static ConstructorHelpers::FObjectFinder<UWeaponType> WeaponTypeDataFinder(TEXT("/Script/ARPG_Hunter.WeaponType'/Game/03-Data/WeaponTypeData.WeaponTypeData'"));
	if (WeaponTypeDataFinder.Succeeded())
		WeaponTypeData = WeaponTypeDataFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_MonsterData.DT_MonsterData'"));
	if (MonsterDataTableFinder.Succeeded())
		MonsterDataTable = MonsterDataTableFinder.Object;

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

FMonsterData* UDataManager::GetMonsterData(const FName& _id) const
{
	return MonsterDataTable->FindRow<FMonsterData>(_id, TEXT("Monster Data Table Search"));
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

void UDataManager::GetAllItemProduct(TArray<struct FItemProductData*>& _outArr) const
{
	ItemProductDataTable->GetAllRows(TEXT("Item Product Data Table Search"), _outArr);
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