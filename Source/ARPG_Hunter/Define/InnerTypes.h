// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InnerTypes.generated.h"

USTRUCT()
struct FActionEventEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<class UEffectData>> Effects;
};