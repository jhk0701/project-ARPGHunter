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

	void SetSectionName(const FName& _name) { SectionName = _name; };

private:
	// Ability 캐릭터가 하는 구체적 동작
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> Montage;
	
	UPROPERTY(EditDefaultsOnly)
	FName SectionName{ NAME_None };

	// 데이터 -> UAction의 대체가 되나? 
	// 행위 수행 Ability
	// 행위에 필요한 데이터는 UAction 그대로 사용하던가
	// 여기선 행동, 행위에 집중
	UPROPERTY(EditDefaultsOnly)
	float PlayRate{ 1.0f };
};
