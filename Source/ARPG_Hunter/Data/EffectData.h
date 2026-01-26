// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EffectData.generated.h"

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	// 자원 회복
	RECOVER_HEALTH = 0		UMETA(DisplayName = "Recover Health"),
	RECOVER_STAMINA			UMETA(DisplayName = "Recover Stamina"),
	RECOVER_SKILL			UMETA(DisplayName = "Recover Skill"),

	// 스탯 버프
	// 특수 이벤트
};

/**
 * 
 */
USTRUCT(BlueprintType)
struct ARPG_HUNTER_API FEffectData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EEffectType Type;


};
