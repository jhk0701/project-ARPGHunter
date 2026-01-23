// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Action.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UAction : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint16 StaminaUsage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;
};
