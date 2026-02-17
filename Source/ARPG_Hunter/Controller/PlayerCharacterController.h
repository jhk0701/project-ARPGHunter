// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

enum class EShortCutType : uint8 
{
	ESC,
	TAB,
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerCharacter> ControlledCharacter;
	UPROPERTY(VisibleAnywhere)
	bool bCursorIsLocked{ false };
	
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
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> ShortCutAction;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	TObjectPtr<UInputMappingContext> GetPlayerIMC() { return PlayerIMC; }
	TObjectPtr<APlayerCharacter> GetControlledPlayer() { return ControlledCharacter; }

	virtual void ShortCut(EShortCutType _key) {};

public:
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
	UFUNCTION()
	void InputShortCut(const FInputActionValue& _value);

	void LockCursor(TSharedPtr<SWidget> _uiToFocus);
	void UnLockCursor();
	bool CursorIsLocked() const { return bCursorIsLocked; }
};
