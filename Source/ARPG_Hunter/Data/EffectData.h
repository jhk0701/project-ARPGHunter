// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EffectData.generated.h"

USTRUCT()
struct FEffectParam
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Common")
	uint32 Value{ 0 };
	UPROPERTY(EditAnywhere, Category = "Common")
	float Duration{ 0.0f };
	UPROPERTY(EditAnywhere, Category = "Common")
	uint8 MaxStack{ 1 };
	UPROPERTY(EditAnywhere, Category = "Repeated")
	float RepeatInterval{ 1.0f };
	UPROPERTY(EditAnywhere, Category = "Target")
	TArray<TObjectPtr<class UEffectData>> TargetEffects;
};

UCLASS()
class ARPG_HUNTER_API UEffectData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere);
	bool bIsDebuff;

	UPROPERTY(EditAnywhere);
	TObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere);
	TObjectPtr<class UParticleSystem> Particle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEffect> Effect; // 효과 클래스

	UPROPERTY(EditAnywhere)
	FEffectParam Param; // 효과값
};
