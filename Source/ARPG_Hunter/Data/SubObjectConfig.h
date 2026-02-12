// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SubObjectConfig.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API USubObjectConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNiagaraSystem> VFX;
};