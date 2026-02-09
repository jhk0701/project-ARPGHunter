// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SWORD		UMETA(DisplayName = "Sword"),
};

FName EnumToName(EWeaponType _value);

UENUM(BlueprintType)
enum class EArmorPart : uint8
{
	HEAD		UMETA(DisplayName = "Head"),
	TOP			UMETA(DisplayName = "Top"),
	BOTTOM		UMETA(DisplayName = "Bottom"),
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NORMAL		UMETA(DisplayName = "Normal"),
	SMASH		UMETA(DisplayName = "Smash"),
	SKILL 		UMETA(DisplayName = "Skill"),

	END			UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EAttackDirection : uint8
{
	FRONT		UMETA(DisplayName = "Front"),
	FRONT_WIDE	UMETA(DisplayName = "Front Wide"),
	AROUND		UMETA(DisplayName = "Around"),
};


UENUM(BlueprintType)
enum class EActionProcess : uint8
{
	START			UMETA(DisplayName = "Start"),
	IN_PROGRESS		UMETA(DisplayName = "InProgress"),
	COMPLETE		UMETA(DisplayName = "Complete"),

	NONE			UMETA(Hidden)
};

FName EnumToName(EActionProcess _value);

UENUM(BlueprintType)
enum class EActionInput : uint8
{
	NORMAL		UMETA(DisplayName = "Normal"),
	HOLD		UMETA(DisplayName = "Hold"),
};

UENUM()
enum class ECharacterStatType : uint8
{
	HEALTH						UMETA(DisplayName = "Health"),
	STAMINA						UMETA(DisplayName = "Stamina"),
	SKILL						UMETA(DisplayName = "Skill"),
	ATTACK						UMETA(DisplayName = "Attack"),
	DEFENSE						UMETA(DisplayName = "Defense"),
	CRITICAL_PERCENT			UMETA(DisplayName = "Critical Per"), // 크리티컬 확률
	CRITICAL_DAMAGE_PERCENT		UMETA(DisplayName = "Critical Damage Per"), // 크리티컬 시, 증가 데미지

	END							UMETA(Hidden),
};

UENUM()
enum class ECharacterResourceType : uint8
{
	HEALTH		UMETA(DisplayName = "Health"),
	STAMINA		UMETA(DisplayName = "Stamina"),
	SKILL		UMETA(DisplayName = "Skill"),

	END			UMETA(Hidden),
};


UENUM(BlueprintType)
enum class EMonsterType : uint8
{
	MELEE		UMETA(DisplayName = "Melee"),
	RANGED		UMETA(DisplayName = "Ranged"),
	BOSS		UMETA(DisplayName = "Boss"),

	END			UMETA(DisplayName = "End")
};

FString EnumToString(EMonsterType _type);

UENUM(BlueprintType)
enum class EMonsterAttackType : uint8 
{
	MELEE		UMETA(DisplayName = "Melee"),
	RANGED		UMETA(DisplayName = "Ranged"),
	SKILL		UMETA(DisplayName = "Skill"),
	GIMIC		UMETA(DisplayName = "Gimic"),

	END
};