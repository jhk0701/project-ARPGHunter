// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EquipmentSuccessData.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UEquipmentSuccessData : public UDataAsset
{
	GENERATED_BODY()
public:
	// 계승 시, 줄어드는 강화 수치
	UPROPERTY(EditAnywhere)
	uint8 SubtractOnSuccession{ 2 };
	UPROPERTY(EditAnywhere)
	uint32 GoldCost{ 500 };

	// 계승에 필요한 최소 수치
	uint8 GetRequiredGradeForSuccession() const { return SubtractOnSuccession + 1; } 
};
