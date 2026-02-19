// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerConfig.generated.h"

enum class ECharacterStatType : uint8;
enum class EEquipmentType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UPlayerConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint32> InitStat;
	UPROPERTY(EditAnywhere)
	TMap<EEquipmentType, TObjectPtr<USkeletalMesh>> MapDefalutMesh;
};
