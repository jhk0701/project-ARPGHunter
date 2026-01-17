// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponTypeProfile.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UWeaponTypeProfile : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimInstance;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HitMontage;
};
