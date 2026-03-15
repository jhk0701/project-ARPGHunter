// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkillUpgrade.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API USkillUpgrade : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText NameText;
	UPROPERTY(EditAnywhere)
	FText DescText;
	UPROPERTY(EditAnywhere)
	uint8 TargetIndex;
	UPROPERTY(EditAnywhere)
	uint8 Cost{ 1 };
	UPROPERTY(EditAnywhere)
	uint8 MaxUpgrade{ 1 };

	virtual void AdjustNode() {};
};

UCLASS()
class ARPG_HUNTER_API USkillNodeEnabler : public USkillUpgrade
{
	GENERATED_BODY()
public:
	
};

UCLASS()
class ARPG_HUNTER_API USkillNodeEffectModifier : public USkillUpgrade
{
	GENERATED_BODY()
public:

};

UCLASS()
class ARPG_HUNTER_API USkillNodeEffectExtender : public USkillUpgrade
{
	GENERATED_BODY()
public:

};

UCLASS()
class ARPG_HUNTER_API USkillNodeSpecModifier : public USkillUpgrade
{
	GENERATED_BODY()
public:

};