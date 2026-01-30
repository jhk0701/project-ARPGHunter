// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;
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
	TObjectPtr<APlayerCharacter> ControlledCharacter;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputMappingContext> PlayerIMC;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> RotateAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> InteractAction;

public:
	APlayerCharacterController();
	
	UFUNCTION()
	void InputMove(const FInputActionValue& _value);
	UFUNCTION()
	void InputMoveEnd(const FInputActionValue& _value);
	UFUNCTION()
	void InputRotate(const FInputActionValue& _value);
	UFUNCTION()
	void InputSprintStart(const FInputActionValue& _value);
	UFUNCTION()
	void InputSprintEnd(const FInputActionValue& _value);
	UFUNCTION()
	void InputInteract(const FInputActionValue& _value);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	TObjectPtr<UInputMappingContext> GetPlayerIMC() { return PlayerIMC; }
	TObjectPtr<APlayerCharacter> GetControlledPlayer() { return ControlledCharacter; }
};
