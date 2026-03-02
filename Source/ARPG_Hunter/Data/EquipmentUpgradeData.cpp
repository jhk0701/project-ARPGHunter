// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/EquipmentUpgradeData.h"

#include "Define/Enum.h"

FName FEquipmentUpgradeData::GetUpgradeID(uint8 _rank, uint8 _grade, EEquipmentType _type)
{
	FString str = FString::Printf(TEXT("%d_%d_%s"), _rank, _grade, *EnumToString(_type));
	return FName(str);
}
