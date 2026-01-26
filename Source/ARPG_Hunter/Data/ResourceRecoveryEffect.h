// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/Effect.h"
#include "ResourceRecoveryEffect.generated.h"

UENUM(BlueprintType)
enum class EResourceType : uint8
{
	HEALTH		UMETA(DisplayName = "Health"),
	STAMINA		UMETA(DisplayName = "Stamina"),
	SKILL		UMETA(DisplayName = "Skill"),
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UResourceRecoveryEffect : public UEffect
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EResourceType TargetType;

public:
	void Activate(const FEffectContext& _context) override;
};
