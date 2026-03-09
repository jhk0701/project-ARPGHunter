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

public:
	APlayerCombatController();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> AttackAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> QuickSlotAction;

protected:
	virtual void OnPossess(APawn* _pawn) override;
	virtual void SetupInputComponent() override;

public:
	UFUNCTION()
	void InputDodge(const FInputActionValue& _value);
	UFUNCTION()
	void InputAttack(const FInputActionValue& _value);
	UFUNCTION()
	void InputAttackEnd(const FInputActionValue& _value);
	UFUNCTION()
	void InputQuickSlot(const FInputActionValue& _value);

	virtual void LockCursor(TSharedPtr<SWidget> _uiToFocus) override;
};
