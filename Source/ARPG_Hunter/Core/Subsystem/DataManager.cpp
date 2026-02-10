// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystem/DataManager.h"
#include "Engine/DataTable.h"
#include "Define/Enum.h"
#include "Data/WeaponTypeData.h"
#include "Data/MonsterData.h"
#include "Data/StageData.h"

UDataManager::UDataManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponTypeDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_WeaponTypeData.DT_WeaponTypeData'"));
	if (WeaponTypeDataTableFinder.Succeeded())
		WeaponTypeDataTable = WeaponTypeDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_MonsterData.DT_MonsterData'"));
	if (MonsterDataTableFinder.Succeeded())
		MonsterDataTable = MonsterDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterActionDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_MonsterActionData.DT_MonsterActionData'"));
	if (MonsterActionDataTableFinder.Succeeded())
		MonsterActionDataTable = MonsterActionDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> RegionDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_RegionData.DT_RegionData'"));
	if(RegionDataTableFinder.Succeeded())
		RegionDataTable = RegionDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> StageDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_StageData.DT_StageData'"));
	if (StageDataTableFinder.Succeeded())
		StageDataTable = StageDataTableFinder.Object;
}

FWeaponTypeData* UDataManager::GetWeaponTypeData(EWeaponType _type) const
{
	return WeaponTypeDataTable->FindRow<FWeaponTypeData>(EnumToName(_type), TEXT("WeaponType Data Table Search"));
}

FMonsterData* UDataManager::GetMonsterData(const FName& _id) const
{
	return MonsterDataTable->FindRow<FMonsterData>(_id, TEXT("Monster Data Table Search"));
}

FMonsterAction* UDataManager::GetMonsterActionData(const FName& _id) const
{
	return MonsterActionDataTable->FindRow<FMonsterAction>(_id, TEXT("Monster Action Data Table Search"));
}

FRegionData* UDataManager::GetRegionData(const FName& _id) const
{
	return RegionDataTable->FindRow<FRegionData>(_id, TEXT("Region Data Table Search"));
}

FStageData* UDataManager::GetStageData(const FName& _id) const
{
	return StageDataTable->FindRow<FStageData>(_id, TEXT("Stage Data Table Search"));
}
