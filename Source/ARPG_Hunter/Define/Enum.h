// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	ITEM				UMETA(DisplayName = "Item"),
	CONSUMABLE			UMETA(DisplayName = "Consumable Item"),
	EQUIPABLE			UMETA(Hidden), // 장비 계열이라고 표시
	WEAPON = EQUIPABLE	UMETA(DisplayName = "Weapon Item"),
	ARMOR				UMETA(DisplayName = "Armor Item"),
	
	END					UMETA(Hidden)
};
FString EnumToString(EItemType _type);
FText EnumToText(EItemType _type);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	SWORD		UMETA(DisplayName = "Sword"),
	HAND_GUN	UMETA(DisplayName = "Hand Gun"),

	END			UMETA(Hidden)
};
FName EnumToName(EWeaponType _value);
FText EnumToText(EWeaponType _value);

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	WEAPON		UMETA(DisplayName = "Weapon"),

	HEAD		UMETA(DisplayName = "Head"),
	TOP			UMETA(DisplayName = "Top"),
	BOTTOM		UMETA(DisplayName = "Bottom"),

	END			UMETA(Hidden)
};
FString EnumToString(EEquipmentType _type);

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	NORMAL		UMETA(DisplayName = "Normal"),
	SMASH		UMETA(DisplayName = "Smash"),
	SKILL 		UMETA(DisplayName = "Skill"),

	END			UMETA(Hidden)
};
FString EnumToString(EAttackType _type);

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
	COMBO		UMETA(DisplayName = "Combo"),
};

UENUM(BlueprintType)
enum class EActionEvent : uint8 
{
	ON_START			UMETA(DisplayName = "On Start"),
	ON_PROGRESS			UMETA(DisplayName = "On Progress"),

	// 적중 시, 공격 주체에게 부여할 효과
	ON_HIT				UMETA(DisplayName = "On Hit"),	
	// 적중 시, 적에게 부여할 효과
	ON_ENEMY_HIT		UMETA(DisplayName = "On Enemy Hit"),
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
FText EnumToText(ECharacterStatType _type);

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
	MELEE					UMETA(DisplayName = "Melee"),
	RANGED					UMETA(DisplayName = "Ranged"),
	GIMIC					UMETA(DisplayName = "Gimic"),
	PICK_OFF				UMETA(DisplayName = "PickOff"),

	MELEE_OR_RANGED			UMETA(DisplayName = "MeleeOrRanged"),
	END = MELEE_OR_RANGED	UMETA(Hidden)
};

// 몬스터의 견제 행동 시 방향
UENUM(BlueprintType)
enum class EPickOff : uint8 
{
	BACKWARD		UMETA(DisplayName = "Backward"),
	RIGHTWARD		UMETA(DisplayName = "Rightward"),
	LEFTWARD		UMETA(DisplayName = "Leftward"),
	
	NONE			UMETA(Hidden)
};
FName EnumToName(EPickOff _type);

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
	NORMAL		UMETA(DisplayName = "Normal"),
	DEAD		UMETA(DisplayName = "Dead"),
	GIMIC		UMETA(DisplayName = "Gimic"),
	GROGGY		UMETA(DisplayName = "Groggy"),
};
FName EnumToName(EMonsterState _type);

UENUM(BlueprintType)
enum class EMonsterAlertState : uint8 
{
	IDLE		UMETA(DisplayName = "Idle"),
	SUSPICIOUS	UMETA(DisplayName = "Suspicious"), 
	ALERT		UMETA(DisplayName = "Alert"),
	ENAGE		UMETA(DisplayName = "Engage"),
	RETURN		UMETA(DisplayName = "Return"),
};

UENUM(BlueprintType)
enum class EActorGroup : uint8 
{
	FRIENDLY = 0	UMETA(DisplayName = "Friendly"),
	HOSTILE = 100	UMETA(DisplayName = "Hostile"),
	NEUTRAL = 200	UMETA(DisplayName = "Neutral"),

	DEFAULT = 255	UMETA(DisplayName = "Default"),
	END = DEFAULT	UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EPlayerActionType:uint8 
{
	ATTACK		UMETA(DisplayName = "Attack Action"),
	USE_ITEM	UMETA(DisplayName = "Use Item"),

	END			UMETA(Hidden),
};

enum class EShortCutType : uint8
{
	NONE,

	ESC,
	TAB,
	F1,
	KEY_I,
	KEY_K,

	END
};