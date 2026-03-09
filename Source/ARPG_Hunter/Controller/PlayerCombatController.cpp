// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerCombatController.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

#include "Player/PlayerCharacter.h"

APlayerCombatController::APlayerCombatController()
{
	static ConstructorHelpers::FObjectFinder<UInputAction> DodgeActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Dodge.IA_Dodge'"));
	if (DodgeActionFinder.Succeeded())
		DodgeAction = DodgeActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Attack.IA_Attack'"));
	if (AttackActionFinder.Succeeded())
		AttackAction = AttackActionFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> QuickSlotActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_QuickSlot.IA_QuickSlot'"));
	if (QuickSlotActionFinder.Succeeded())
		QuickSlotAction = QuickSlotActionFinder.Object;
}

void APlayerCombatController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);
	GetControlledPlayer()->SetIsCombat(true);
}

void APlayerCombatController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		InputComp->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCombatController::InputDodge);

		InputComp->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCombatController::InputAttack);
		InputComp->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCombatController::InputAttackEnd);

		InputComp->BindAction(QuickSlotAction, ETriggerEvent::Triggered, this, &APlayerCombatController::InputQuickSlot);
	}
}

void APlayerCombatController::InputDodge(const FInputActionValue& _value)
{
	GetControlledPlayer()->Dodge();
}

void APlayerCombatController::InputAttack(const FInputActionValue& _value)
{
	uint8 Val = static_cast<uint8>(_value.Get<float>()) - 1;
	GetControlledPlayer()->Attack(static_cast<EAttackType>(Val));
}

void APlayerCombatController::InputAttackEnd(const FInputActionValue& _value)
{
	GetControlledPlayer()->AttackEnd();
}

void APlayerCombatController::InputQuickSlot(const FInputActionValue& _value)
{
	uint8 Index = static_cast<uint8>(_value.Get<float>()) - 1;
	GetControlledPlayer()->UseQuickSlot(Index);
}

void APlayerCombatController::LockCursor(TSharedPtr<SWidget> _uiToFocus)
{
	Super::LockCursor(_uiToFocus);

	GetControlledPlayer()->AttackEnd();
}
