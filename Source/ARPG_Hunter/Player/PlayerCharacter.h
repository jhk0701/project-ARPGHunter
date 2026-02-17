// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/Effectable.h"
#include "Interface/AttackNotifyHandler.h"
#include "PlayerCharacter.generated.h"

class UCameraShakeBase;
enum class EAttackType : uint8;
enum class EActionProcess : uint8;

UCLASS()
class ARPG_HUNTER_API APlayerCharacter : public ACharacter, public IHitable, public IAttackNotifyHandler, public IEffectable
{
	GENERATED_BODY()

private:
#pragma region Component
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStatComponent> StatComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UEquipmentComponent> EquipComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPlayerActionComponent> ActionComp;

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

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> InteractWidget;
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

#pragma region Sub Effect
	UPROPERTY(EditAnywhere, Category = "Effect|Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeOnAttack;
	UPROPERTY(EditAnywhere, Category = "Effect|Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeOnHit;
#pragma endregion

#pragma region Interaction

	class IInteractable* CurInteractable;

#pragma endregion

public:
	APlayerCharacter();

private:
	void SmoothRotateToInputDir(float DeltaTime);

	// 기본 데미지 계산
	uint32 CalculateBaseDamage(); 
	bool CalculateCritical(uint32& _outDamage);
	void AdjustDefense(uint32& _outDamage);

	void CheckInteractable();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void Dodge();
	void Attack(EAttackType _eType);
	void AttackEnd();
	void SetInputDirection(FVector2D _dir) 
	{ 
		InputDirection = _dir; 
		InputDirection.Normalize(); 
	}
	const FVector2D& GetInputDirection() { return InputDirection; }
	void SetIsSprint(bool _isSprint);
	bool GetIsSprint() { return IsSprint; }

	void SetActionProcess(EActionProcess _eProcess);

	// IHitable을(를) 통해 상속됨
	void HitBy(const FHitInfo& _hitInfo) override;
	// IAttackNotifyHandler을(를) 통해 상속됨
	void HandleAttackNotify(uint8 _opt) override;
	
	bool IsDead();

	// IEffectable을(를) 통해 상속됨
	void ApplyEffect(TObjectPtr<class UEffectData> _effectData) override;

	void ShakeCameraOnAttack(float _scale = 1.0f);
	void ShakeCamera(TSubclassOf<UCameraShakeBase> _shakeClass, float _scale = 1.0f);
	void Interact();
};
