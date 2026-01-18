// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/DataManager/DataManager.h"
#include "Engine/DataTable.h"

UDataManager::UDataManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponTypeDataTableFinder(TEXT("/Script/Engine.DataTable'/Game/03-Data/DT_WeaponTypeData.DT_WeaponTypeData'"));
	if (WeaponTypeDataTableFinder.Succeeded())
		WeaponTypeDataTable = WeaponTypeDataTableFinder.Object;
}
