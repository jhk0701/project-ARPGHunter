// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputMappingContext> PlayerIMC;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> RotateAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> DodgeAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> AttackNormalAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> AttackSmashAction;


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class APlayerCharacter> ControlledCharacter;

public:
	APlayerCharacterController();
	
	UFUNCTION()
	void InputMove(const FInputActionValue& _value);
	UFUNCTION()
	void InputMoveEnd(const FInputActionValue& _value);
	UFUNCTION()
	void InputRotate(const FInputActionValue& _value);
	UFUNCTION()
	void InputDodge(const FInputActionValue& _value);
	UFUNCTION()
	void InputSprintStart(const FInputActionValue& _value);
	UFUNCTION()
	void InputSprintEnd(const FInputActionValue& _value);
	UFUNCTION()
	void InputAttackNormal(const FInputActionValue& _value);
	UFUNCTION()
	void InputAttackSmash(const FInputActionValue& _value);

protected:
	void BeginPlay() override;
	void SetupInputComponent() override;
};
