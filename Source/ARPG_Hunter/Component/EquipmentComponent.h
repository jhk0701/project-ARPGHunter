// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	/*UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TObjectPtr<struct FWeaponTypeProfile> WeaponProfile;*/

public:	
	UEquipmentComponent();

	void Init();
	bool IsValid() { return false; } //return WeaponProfile != nullptr; 
	UAnimMontage* GetDodgeMontage();
};
