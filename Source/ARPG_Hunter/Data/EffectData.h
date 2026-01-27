// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EffectData.generated.h"

UCLASS()
class ARPG_HUNTER_API UEffectData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere);
	TObjectPtr<UTexture> Icon;

	UPROPERTY(EditAnywhere);
	TObjectPtr<class UParticleSystem> Particle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEffect> Effect; // 효과 내용
};
