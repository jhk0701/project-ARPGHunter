// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

enum class EWeaponType :uint8;
enum class EArmorPart :uint8;
enum class ECharacterStatType :uint8;

/**
 * 
 */
USTRUCT()
struct ARPG_HUNTER_API FItemData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	FString Desc;
	UPROPERTY(EditAnywhere)
	uint16 MaxCount; // 최대 9999개까지 소지 가능
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture> Thumbnail;
};

USTRUCT()
struct ARPG_HUNTER_API FConsumableItemData : public FItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint8 AllowCountOnCombat{10}; // 전투 시, 소지 가능 횟수
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UEffect>> Effects; // 사용 시, 효과
};


USTRUCT()
struct ARPG_HUNTER_API FWeaponItemData : public FItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EWeaponType Type;
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint16> WeaponStat;
};

USTRUCT()
struct ARPG_HUNTER_API FArmorItemData : public FItemData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EArmorPart Type;
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint16> ArmorStat;
};