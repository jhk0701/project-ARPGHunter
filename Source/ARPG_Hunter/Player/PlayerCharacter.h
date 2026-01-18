// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ARPG_HUNTER_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEquipmentComponent> EquipComp;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> HeadMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> TopMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> BottomMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> HandMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> FeetMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComp;
#pragma endregion

#pragma region Attribute
	FVector2D InputDirection{};
	bool IsSprint{ false };

	UPROPERTY(EditAnywhere, Category = "Attribute|Speed")
	float WalkSpeed{ 300.0f };
	UPROPERTY(EditAnywhere, Category = "Attribute|Speed")
	float SprintSpeed{ 600.0f };
#pragma endregion


public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void Dodge();

	void SetInputDirection(FVector2D _dir) { InputDirection = _dir; }
	const FVector2D& GetInputDirection() { return InputDirection; }
	void SetIsSprint(bool _isSprint);
	bool GetIsSprint() { return IsSprint; }
};
