// Fill out your copyright notice in the Description page of Project Settings.


#include "Define/Enum.h"
#include "CoreMinimal.h"

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

FName EnumToName(EActionProcess _value)
{
	switch (_value)
	{
	case EActionProcess::START:
		return FName(TEXT("Start"));
	case EActionProcess::IN_PROGRESS:
		return FName(TEXT("InProgress"));
	case EActionProcess::COMPLETE:
		return FName(TEXT("Complete"));
	case EActionProcess::NONE: __fallthrough;
	default:
		return FName(TEXT("None"));
	}
}

FString EnumToString(EMonsterType _type)
{
	switch (_type)
	{
	case EMonsterType::MELEE:
		return FString(TEXT("Melee"));
	case EMonsterType::RANGED:
		return FString(TEXT("Ranged"));
	case EMonsterType::BOSS:
		return FString(TEXT("Boss"));

	default:
		return FString();
	}
}

FName EnumToName(EGimicType _type)
{
	switch (_type)
	{
	case EGimicType::COUNTER:
		return FName(TEXT("Counter"));
	case EGimicType::STAGGER:
		return FName(TEXT("Stagger"));
	default:
		return NAME_None;
	}
}
