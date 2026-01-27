// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct.generated.h"

USTRUCT()
struct FEffectParam
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Common")
	uint32 Value;
	UPROPERTY(EditAnywhere, Category = "Common")
	float Duration;
	UPROPERTY(EditAnywhere, Category = "Repeated")
	float RepeatInterval{ 1.0f };
};
