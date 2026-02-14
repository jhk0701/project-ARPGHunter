// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystem/DataManager.h"
#include "Engine/DataTable.h"
#include "Define/Enum.h"
#include "Data/WeaponConfig.h"
#include "Data/MonsterData.h"
#include "Data/StageData.h"

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
