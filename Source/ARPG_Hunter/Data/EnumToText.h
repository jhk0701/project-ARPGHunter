// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnumToText.generated.h"

enum class EItemType :uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UEnumToText : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<EItemType, FText> ItemType;
};
