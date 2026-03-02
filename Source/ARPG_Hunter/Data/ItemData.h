// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

enum class EWeaponType :uint8;
enum class EEquipmentType :uint8;
enum class EItemType : uint8;
enum class ECharacterStatType :uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UItemConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString Name;
	UPROPERTY(EditAnywhere)
	FString Desc;
	UPROPERTY(EditAnywhere)
	uint16 MaxAmount; // 최대 9999개까지 소지 가능
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Thumbnail;
};

UCLASS()
class ARPG_HUNTER_API UConsumableItemConfig : public UItemConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint8 AllowCountOnCombat{10}; // 전투 시, 소지 가능 횟수
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UEffectData>> Effects; // 사용 시, 효과

	UPROPERTY(EditAnywhere)
	TObjectPtr<UParticleSystem> VFX;
};

UCLASS()
class ARPG_HUNTER_API UEquipmentItemConfig : public UItemConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint8 Rank;

	UPROPERTY(EditAnywhere)
	EEquipmentType Type;
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint32> Stat;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;
};

UCLASS()
class ARPG_HUNTER_API UWeaponItemConfig : public UEquipmentItemConfig
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
};

USTRUCT()
struct ARPG_HUNTER_API FItemData : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EItemType Type;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UItemConfig> Item;
};
