// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseConfig.h"
#include "AISenseConfig_PlayerAction.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = AI, DisplayName = "AI Player Action sense config")
class ARPG_HUNTER_API UAISenseConfig_PlayerAction : public UAISenseConfig
{
	GENERATED_BODY()
	
public:
	UAISenseConfig_PlayerAction();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sense", config, meta = (UIMin = 0.0, ClampMin = 0.0))
	float RecogRange = 500.0f;
	
	virtual TSubclassOf<UAISense> GetSenseImplementation() const override;
};
