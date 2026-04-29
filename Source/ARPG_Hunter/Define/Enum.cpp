// Fill out your copyright notice in the Description page of Project Settings.

#include "Define/Enum.h"
#include "CoreMinimal.h"

FString EnumToString(EItemType _type)
{
	switch (_type)
	{
	case EItemType::ITEM:
		return TEXT("Item"); // IT_Item
	case EItemType::CONSUMABLE:
		return TEXT("Consumable"); // IT_Consumable
	case EItemType::WEAPON:
		return TEXT("Weapon");	// IT_Weapon
	case EItemType::ARMOR:
		return TEXT("Armor");  // IT_Armor
	default:
		return TEXT("None");
	}
}

FText EnumToText(EItemType _type)
{
	switch (_type)
	{
	case EItemType::ITEM:
		return FText::FromString(TEXT("일반 재료"));
	case EItemType::CONSUMABLE:
		return FText::FromString(TEXT("소비품"));
	case EItemType::WEAPON:
		return FText::FromString(TEXT("무기"));
	case EItemType::ARMOR:
		return FText::FromString(TEXT("방어구"));
	default:
		return FText();
	}
}

FName EnumToName(EWeaponType _value)
{
	switch (_value)
	{
	case EWeaponType::SWORD:
		return FName(TEXT("Sword"));
	case EWeaponType::HAND_GUN:
		return FName(TEXT("Hand Gun"));
	default:
		return FName(TEXT("None"));
	}
}

FText EnumToText(EWeaponType _value)
{
	switch (_value)
	{
	case EWeaponType::SWORD:
		return FText::FromString(TEXT("검"));
	case EWeaponType::HAND_GUN:
		return FText::FromString(TEXT("핸드건"));
	default:
		return FText::FromString(TEXT("None"));
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

FString EnumToString(EAttackType _type)
{
	switch (_type)
	{
	case EAttackType::NORMAL:
		return TEXT("Normal");
	case EAttackType::SMASH:
		return TEXT("Smash");
	case EAttackType::SKILL:
		return TEXT("Skill");
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
		return FName(TEXT("Stat_Health"));
	case ECharacterStatType::STAMINA:
		return FName(TEXT("Stat_Stamina"));
	case ECharacterStatType::SKILL:
		return FName(TEXT("Stat_Skill"));
	case ECharacterStatType::ATTACK:
		return FName(TEXT("Stat_Attack"));
	case ECharacterStatType::DEFENSE:
		return FName(TEXT("Stat_Defense"));
	case ECharacterStatType::CRITICAL_PERCENT:
		return FName(TEXT("Stat_CriticalPer"));
	case ECharacterStatType::CRITICAL_DAMAGE_PERCENT:
		return FName(TEXT("Stat_CriticalDmg"));
	default:
		return NAME_None;
	}
}

FText EnumToText(ECharacterStatType _type)
{
	switch (_type)
	{
	case ECharacterStatType::HEALTH:
		return FText::FromString(TEXT("Health"));
	case ECharacterStatType::STAMINA:
		return FText::FromString(TEXT("Stamina"));
	case ECharacterStatType::SKILL:
		return FText::FromString(TEXT("Skill"));
	case ECharacterStatType::ATTACK:
		return FText::FromString(TEXT("Attack"));
	case ECharacterStatType::DEFENSE:
		return FText::FromString(TEXT("Defense"));
	case ECharacterStatType::CRITICAL_PERCENT:
		return FText::FromString(TEXT("Critical Percent"));
	case ECharacterStatType::CRITICAL_DAMAGE_PERCENT:
		return FText::FromString(TEXT("Critical Damage"));
	default:
		return FText();
	}
}

FName EnumToName(EPickOff _type)
{
	switch (_type)
	{
	case EPickOff::BACKWARD:
		return FName(TEXT("Backward"));
	case EPickOff::RIGHTWARD:
		return FName(TEXT("Rightward"));
	case EPickOff::LEFTWARD:
		return FName(TEXT("Leftward"));
	case EPickOff::NONE: __fallthrough;
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