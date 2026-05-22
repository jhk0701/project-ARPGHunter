// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_CharacterMove.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UGA_CharacterMove : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_CharacterMove();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
