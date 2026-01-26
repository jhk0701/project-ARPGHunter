// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerCharacterController.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Define/Enum.h"
#include "Player/PlayerCharacter.h"

APlayerCharacterController::APlayerCharacterController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> PlayerIMCFinder(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/04-Input/IMC_Player.IMC_Player'"));
	if (PlayerIMCFinder.Succeeded())
		PlayerIMC = PlayerIMCFinder.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Move.IA_Move'"));
	if (MoveActionFinder.Succeeded())
		MoveAction = MoveActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> DodgeActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Dodge.IA_Dodge'"));
	if (DodgeActionFinder.Succeeded())
		DodgeAction = DodgeActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> RotateActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Rotate.IA_Rotate'"));
	if (RotateActionFinder.Succeeded())
		RotateAction = RotateActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Sprint.IA_Sprint'"));
	if (SprintActionFinder.Succeeded())
		SprintAction = SprintActionFinder.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Attack.IA_Attack'"));
	if (AttackActionFinder.Succeeded())
		AttackAction = AttackActionFinder.Object;
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == PlayerIMC)
		return;

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsytem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) 
		InputSubsytem->AddMappingContext(PlayerIMC, 0);

	ControlledCharacter = Cast<APlayerCharacter>(GetPawn());
}

void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		InputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::InputMove);
		InputComp->BindAction(MoveAction, ETriggerEvent::Completed, this, &APlayerCharacterController::InputMoveEnd);
		
		InputComp->BindAction(RotateAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::InputRotate);
		
		InputComp->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacterController::InputSprintStart);
		InputComp->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacterController::InputSprintEnd);
		
		InputComp->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::InputDodge);

		InputComp->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::InputAttack);
		InputComp->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacterController::InputAttackEnd);
	}	
}

void APlayerCharacterController::InputMove(const FInputActionValue& _value)
{
	if (ControlledCharacter->IsDead())
		return;

	FVector2D Dir = _value.Get<FVector2D>();
	FVector Fwd = GetTransformComponent()->GetForwardVector();
	Fwd.Z = 0;
	Fwd.Normalize();
	FVector Rht = GetTransformComponent()->GetRightVector();
	Rht.Z = 0;
	Rht.Normalize();

	ControlledCharacter->AddMovementInput(Fwd, Dir.X);
	ControlledCharacter->AddMovementInput(Rht, Dir.Y);
	ControlledCharacter->SetInputDirection(Dir);
}

void APlayerCharacterController::InputMoveEnd(const FInputActionValue& _value)
{
	ControlledCharacter->SetInputDirection(FVector2D::ZeroVector);
}

void APlayerCharacterController::InputRotate(const FInputActionValue& _value)
{
	FVector2D Rot = _value.Get<FVector2D>();
	AddPitchInput(Rot.X);
	AddYawInput(Rot.Y);
}

void APlayerCharacterController::InputSprintStart(const FInputActionValue& _value)
{
	ControlledCharacter->SetIsSprint(true);
}

void APlayerCharacterController::InputSprintEnd(const FInputActionValue& _value)
{
	ControlledCharacter->SetIsSprint(false);
}

void APlayerCharacterController::InputDodge(const FInputActionValue& _value)
{
	ControlledCharacter->Dodge();
}

void APlayerCharacterController::InputAttack(const FInputActionValue& _value)
{
	uint8 val = static_cast<uint8>(_value.Get<float>()) - 1;
	ControlledCharacter->Attack(static_cast<EAttackType>(val));
}

void APlayerCharacterController::InputAttackEnd(const FInputActionValue& _value)
{
	ControlledCharacter->AttackEnd();
}
