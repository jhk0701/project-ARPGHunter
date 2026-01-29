// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/PlayerCharacterController.h"
#include "PlayerCombatController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API APlayerCombatController : public APlayerCharacterController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> AttackAction;

public:
	APlayerCombatController();

	UFUNCTION()
	void InputDodge(const FInputActionValue& _value);
	UFUNCTION()
	void InputAttack(const FInputActionValue& _value);
	UFUNCTION()
	void InputAttackEnd(const FInputActionValue& _value);

protected:
	virtual void SetupInputComponent() override;
};
