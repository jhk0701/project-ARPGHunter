// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DataManager/DataManager.h"
#include "Engine/DataTable.h"
#include "Define/Enum.h"
#include "Data/WeaponTypeData.h"

UDataManager::UDataManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponTypeDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_WeaponTypeData.DT_WeaponTypeData'"));
	if (WeaponTypeDataTableFinder.Succeeded())
		WeaponTypeDataTable = WeaponTypeDataTableFinder.Object;
}

FWeaponTypeData* UDataManager::GetWeaponTypeData(EWeaponType _type)
{
	return WeaponTypeDataTable->FindRow<FWeaponTypeData>(EnumToName(_type), TEXT("WeaponType Data Table Search"));
}
