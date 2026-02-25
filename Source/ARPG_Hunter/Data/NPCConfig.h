// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NPCConfig.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UNPCConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<USkeletalMesh> Skeletal;

};
