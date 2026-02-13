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

FName EnumToName(EGimicType _type)
{
	switch (_type)
	{
	case EGimicType::COUNTER:
		return FName(TEXT("Counter"));
	case EGimicType::STAGGER:
		return FName(TEXT("Stagger"));

	case EGimicType::END:
		return FName(TEXT("End"));
	default:
		return NAME_None;
	}
}

FName EnumToName(EMonsterState _type)
{
	switch (_type)
	{
	case EMonsterState::NORMAL:
		return FName(TEXT("Normal"));
	case EMonsterState::DEAD:
		return FName(TEXT("Dead"));
	case EMonsterState::GIMIC:
		return FName(TEXT("Gimic"));
	case EMonsterState::GROGGY:
		return FName(TEXT("Groggy"));
	default:
		return NAME_None;
	}
}
