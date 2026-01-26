// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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