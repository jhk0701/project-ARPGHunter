// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	ITEM				UMETA(DisplayName = "Item"),
	CONSUMABLE			UMETA(DisplayName = "Consumable Item"),
	
	WEAPON				UMETA(DisplayName = "Weapon"),
	ARMOR				UMETA(DisplayName = "Armor"),
	EQUIPABLE = WEAPON	UMETA(Hidden), // Weapon 이하는 모두 Equipable 계열이라 표시
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SWORD		UMETA(DisplayName = "Sword"),
};
FName EnumToName(EWeaponType _value);

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	WEAPON		UMETA(DisplayName = "Weapon"),

	HEAD		UMETA(DisplayName = "Head"),
	TOP			UMETA(DisplayName = "Top"),
	BOTTOM		UMETA(DisplayName = "Bottom"),

	END			UMETA(Hidden)
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
enum class EAttackDetailType : uint8
{
	MELEE_FRONT					UMETA(DisplayName = "Melee Front"),
	MELEE_AROUND				UMETA(DisplayName = "Melee Around"),
	MELEE_END = MELEE_AROUND	UMETA(Hidden),

	RANGED_DIRECTIONAL			UMETA(DisplayName = "Ranged Directional"),
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
FName EnumToName(ECharacterStatType _type);

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
	REGULAR		UMETA(DisplayName = "Regular"),
	BOSS		UMETA(DisplayName = "Boss"),

	END			UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMonsterAttackType : uint8 
{
	MELEE		UMETA(DisplayName = "Melee"),
	RANGED		UMETA(DisplayName = "Ranged"),
	GIMIC		UMETA(DisplayName = "Gimic"),

	END
};

UENUM(BlueprintType)
enum class EGimicType : uint8 
{
	COUNTER		UMETA(DisplayName = "Counter"),
	STAGGER		UMETA(DisplayName = "Stagger"),

	END			UMETA(DisplayName = "End")
};
FName EnumToName(EGimicType _type);

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	NORMAL,
	DEAD,

	GIMIC,
	GROGGY,
};
FName EnumToName(EMonsterState _type);