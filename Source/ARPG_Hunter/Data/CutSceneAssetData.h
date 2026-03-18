// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CutSceneAssetData.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UCutSceneAssetData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class ULevelSequence> LevelSequence;
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AActor>> LevelSequenceRefs;
};
