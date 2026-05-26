// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_Action.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UGA_Action : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Action();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION()
	void OnComplete();
	UFUNCTION()
	void OnCanceled();

private:
	// Ability 캐릭터가 하는 구체적 동작
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> Montage;

	// 데이터 -> UAction의 대체가 되나? -> 데이터의 검열
	// 여기선 행동, 행위에 집중
	UPROPERTY(EditDefaultsOnly)
	float PlayRate{ 1.0f };
};
