// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/AttackNotifyHandler.h"
#include "PlayerCharacter.generated.h"

enum class EAttackType : uint8;

UCLASS()
class ARPG_HUNTER_API APlayerCharacter : public ACharacter, public IHitable, public IAttackNotifyHandler
{
	GENERATED_BODY()

private:
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerStatComponent> StatComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEquipmentComponent> EquipComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UActionComponent> ActionComp;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> HeadMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> TopMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> BottomMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh|Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComp;
#pragma endregion

#pragma region Attribute

	FVector2D InputDirection{};
	UPROPERTY(EditAnywhere, Category = "Attribute|Rotate")
	float RotateSpeedToInputDir{ 10.0f };

	bool IsSprint{ false };

	UPROPERTY(EditAnywhere, Category = "Attribute|Speed")
	float WalkSpeed{ 300.0f };
	UPROPERTY(EditAnywhere, Category = "Attribute|Speed")
	float SprintSpeed{ 600.0f };

#pragma endregion

public:
	APlayerCharacter();

private:
	void SmoothRotateToInputDir(float DeltaTime);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void InputDodge();
	void InputAttack(EAttackType _type);

	void SetInputDirection(FVector2D _dir) 
	{ 
		InputDirection = _dir; 
		InputDirection.Normalize(); 
	}
	const FVector2D& GetInputDirection() { return InputDirection; }
	void SetIsSprint(bool _isSprint);
	bool GetIsSprint() { return IsSprint; }
	void EnableNextAction(bool _enable);

	// IHitable을(를) 통해 상속됨
	void HitBy(const FHitInfo& _hitInfo) override;
	// IAttackNotifyHandler을(를) 통해 상속됨
	void HandleAttackNotify(EAttackDirection _eAttackDir) override;

	bool IsDead();
	void OnDead();
};
