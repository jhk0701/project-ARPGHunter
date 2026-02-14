// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

enum class EWeaponType :uint8;
enum class EArmorPart :uint8;
enum class ECharacterStatType :uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UItem : public UDataAsset
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

UCLASS()
class ARPG_HUNTER_API UConsumableItem : public UItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint8 AllowCountOnCombat{10}; // 전투 시, 소지 가능 횟수
	UPROPERTY(EditAnywhere)
	float Cooldown{ 5.0f }; // 사용 쿨타임
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UEffectData>> Effects; // 사용 시, 효과
};

UCLASS(Abstract)
class ARPG_HUNTER_API UEquipableItem : public UItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint16> Stat;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;
};

UCLASS()
class ARPG_HUNTER_API UWeaponItem : public UEquipableItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EWeaponType Type;
};

UCLASS()
class ARPG_HUNTER_API UArmorItem : public UEquipableItem
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EArmorPart Type;
};

USTRUCT()
struct ARPG_HUNTER_API FItemData : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UItem> Item;
};