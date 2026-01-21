// Fill out your copyright notice in the Description page of Project Settings.


#include "Define/Enum.h"

FName EnumToName(EWeaponType _value)
{
	switch (_value)
	{
	case EWeaponType::SWORD:
		return FName(TEXT("Sword"));
	default:
		return FName(TEXT("None"));
	}
}
