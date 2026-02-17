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

FString EnumToString(EEquipmentType _type)
{
	switch (_type)
	{
	case EEquipmentType::WEAPON:
		return TEXT("Weapon");
	case EEquipmentType::HEAD:
		return TEXT("Head");
	case EEquipmentType::TOP:
		return TEXT("Top");
	case EEquipmentType::BOTTOM:
		return TEXT("Bottom");
	default:
		return FString();
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

FName EnumToName(ECharacterStatType _type)
{
	switch (_type)
	{
	case ECharacterStatType::HEALTH:
		return FName(TEXT("Health"));
	case ECharacterStatType::STAMINA:
		return FName(TEXT("Stamina"));
	case ECharacterStatType::SKILL:
		return FName(TEXT("Skill"));
	case ECharacterStatType::ATTACK:
		return FName(TEXT("Attack"));
	case ECharacterStatType::DEFENSE:
		return FName(TEXT("Defense"));
	case ECharacterStatType::CRITICAL_PERCENT:
		return FName(TEXT("Critical"));
	case ECharacterStatType::CRITICAL_DAMAGE_PERCENT:
		return FName(TEXT("Critical Damage"));
	default:
		return NAME_None;
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
