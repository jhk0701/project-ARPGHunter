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
	static ConstructorHelpers::FObjectFinder<UInputAction> RotateActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Rotate.IA_Rotate'"));
	if (RotateActionFinder.Succeeded())
		RotateAction = RotateActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> SprintActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Sprint.IA_Sprint'"));
	if (SprintActionFinder.Succeeded())
		SprintAction = SprintActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> InteractActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_Interact.IA_Interact'"));
	if (InteractActionFinder.Succeeded())
		InteractAction = InteractActionFinder.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> ShortCurActionFinder(TEXT("/Script/EnhancedInput.InputAction'/Game/04-Input/IA_ShortCut.IA_ShortCut'"));
	if (ShortCurActionFinder.Succeeded())
		ShortCutAction = ShortCurActionFinder.Object;
}

void APlayerCharacterController::OnPossess(APawn* _pawn)
{
	Super::OnPossess(_pawn);

	if (nullptr == PlayerIMC)
		return;

	ControlledCharacter = Cast<APlayerCharacter>(_pawn);

	if (UEnhancedInputLocalPlayerSubsystem* InputSubsytem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		InputSubsytem->AddMappingContext(PlayerIMC, 0);
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

		InputComp->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::InputInteract);
		InputComp->BindAction(ShortCutAction, ETriggerEvent::Triggered, this, &APlayerCharacterController::InputShortCut);
	}	
}


void APlayerCharacterController::InputMove(const FInputActionValue& _value)
{
	if (!ControlledCharacter || ControlledCharacter->IsDead() || bCursorIsLocked)
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
	if (!ControlledCharacter) return;

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
	if (!ControlledCharacter) return;

	ControlledCharacter->SetIsSprint(true);
}

void APlayerCharacterController::InputSprintEnd(const FInputActionValue& _value)
{
	if (!ControlledCharacter) return;

	ControlledCharacter->SetIsSprint(false);
}

void APlayerCharacterController::InputInteract(const FInputActionValue& _value)
{
	if (!ControlledCharacter) return;

	ControlledCharacter->Interact();
}

void APlayerCharacterController::InputShortCut(const FInputActionValue& _value)
{
	if (!ControlledCharacter) return;

	EShortCutType Type = static_cast<EShortCutType>(_value.Get<float>());
	ShortCut(Type);
}


void APlayerCharacterController::LockCursor(TSharedPtr<SWidget> _uiToFocus)
{
	if(ControlledCharacter)
		ControlledCharacter->SetInputDirection(FVector2D::ZeroVector);

	bCursorIsLocked = true;
	
	// 커서 출력
	SetShowMouseCursor(bCursorIsLocked); 

	// 마우스 클릭 활성화
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(_uiToFocus);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	
	SetInputMode(InputMode);
}

void APlayerCharacterController::UnLockCursor()
{
	bCursorIsLocked = false;

	// 커서 가리기
	SetShowMouseCursor(bCursorIsLocked);

	// 마우스 클릭 비활성화
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
