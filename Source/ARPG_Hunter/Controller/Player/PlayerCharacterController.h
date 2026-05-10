// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCharacterController.generated.h"

class APlayerCharacter;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

enum class EShortCutType : uint8;

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
	TObjectPtr<UInputAction> ShortCutAction_ESC;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> ShortCutAction_Tab;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> ShortCutAction_F1;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> ShortCutAction_Key_I;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UInputAction> ShortCutAction_Key_K;
	

protected:
	virtual void OnPossess(APawn* _pawn) override;
	virtual void SetupInputComponent() override;

	TObjectPtr<UInputMappingContext> GetPlayerIMC() { return PlayerIMC; }
	TObjectPtr<APlayerCharacter> GetControlledPlayer() { return ControlledCharacter; }

	virtual void ShortCut(EShortCutType _key);

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
	void InputShortCutESC(const FInputActionValue& _value);
	UFUNCTION()
	void InputShortCutTAB(const FInputActionValue& _value);
	UFUNCTION()
	void InputShortCutF1(const FInputActionValue& _value);
	UFUNCTION()
	void InputShortCutI(const FInputActionValue& _value);
	UFUNCTION()
	void InputShortCutK(const FInputActionValue& _value);

	virtual void LockCursor(TSharedPtr<SWidget> _uiToFocus);
	void UnLockCursor();
	bool CursorIsLocked() const { return bCursorIsLocked; }
};
