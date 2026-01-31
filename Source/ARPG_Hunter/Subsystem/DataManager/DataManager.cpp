// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DataManager/DataManager.h"
#include "Engine/DataTable.h"
#include "Define/Enum.h"
#include "Data/WeaponTypeData.h"
#include "Data/MonsterData.h"

UDataManager::UDataManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponTypeDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_WeaponTypeData.DT_WeaponTypeData'"));
	if (WeaponTypeDataTableFinder.Succeeded())
		WeaponTypeDataTable = WeaponTypeDataTableFinder.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_MonsterData.DT_MonsterData'"));
	if (MonsterDataTableFinder.Succeeded())
		MonsterDataTable = MonsterDataTableFinder.Object;
}

FWeaponTypeData* UDataManager::GetWeaponTypeData(EWeaponType _type)
{
	return WeaponTypeDataTable->FindRow<FWeaponTypeData>(EnumToName(_type), TEXT("WeaponType Data Table Search"));
}

FMonsterData* UDataManager::GetMonsterData(FName _id)
{
	return MonsterDataTable->FindRow<FMonsterData>(_id, TEXT("Monster Data Table Search"));
}
