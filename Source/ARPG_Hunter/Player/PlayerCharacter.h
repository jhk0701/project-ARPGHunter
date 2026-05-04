// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARPGCharacterBase.h"
#include "GenericTeamAgentInterface.h"
#include "PlayerCharacter.generated.h"

class UCameraShakeBase;
enum class EAttackType : uint8;
enum class EActionProcess : uint8;
enum class EPlayerActionType : uint8;
enum class EEquipmentType :uint8;
enum class EActorGroup : uint8;

UCLASS()
class ARPG_HUNTER_API APlayerCharacter : public AARPGCharacterBase,	public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	APlayerCharacter();

private:
#pragma region Component

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TMap<EEquipmentType, TObjectPtr<USkeletalMeshComponent>> MapEquipmentMeshComp;
	UPROPERTY(EditAnywhere, Category = "Mesh|Socket")
	FName WeaponSocketOnCombat;
	UPROPERTY(EditAnywhere, Category = "Mesh|Socket")
	FName WeaponSocketOnNonCombat;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class USpringArmComponent> SpringArmComp;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<class UCameraComponent> CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> InteractWidget;

	UPROPERTY(VisibleAnywhere, Category = "AIStimuli")
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> StimuliSourceComp;

#pragma endregion

#pragma region Attribute
	UPROPERTY(EditAnywhere, Category = "Attribute")
	EActorGroup ActorGroup;
	FVector2D InputDirection{};

	UPROPERTY(EditAnywhere, Category = "Attribute|Rotate")
	float RotateSpeedToInputDir{ 10.0f };

	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	bool bIgnoreMoveInput{ false };
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	bool bIsSprint{ false };
	UPROPERTY(VisibleAnywhere, Category = "Attribute")
	bool bIsCombat{ false };

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float AutoOrientToEnemyRadius{ 300.0f };

	UPROPERTY(EditAnywhere, Category = "Attribute|Speed")
	float WalkSpeed{ 300.0f };
	UPROPERTY(EditAnywhere, Category = "Attribute|Speed")
	float SprintSpeed{ 600.0f };

	UPROPERTY(EditAnywhere, Category = "Setting|Interaction")
	float InteractionRange{ 1000.0f };
	UPROPERTY(EditAnywhere, Category = "Setting|Interaction")
	float InteractionSize{ 100.0f };

	UPROPERTY(EditAnywhere, Category = "Setting|PlayerAction")
	TMap<EPlayerActionType, float> RangePerAction;
#pragma endregion

#pragma region Interaction
	
	class IInteractable* CurInteractable;

#pragma endregion

#pragma region QuickSlot

	uint8 UsingQuickSlotIndex;

#pragma endregion

#pragma region Camera
	UPROPERTY(EditAnywhere, Category = "Effect|Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeOnAttack;
	UPROPERTY(EditAnywhere, Category = "Effect|Camera")
	TSubclassOf<UCameraShakeBase> CameraShakeOnHit;

	// AnimNotify의 인스턴스의 생명 주기보다 오래 사용할 수 있기 때문에
	// CurveVector의 포인터를 넘겨 받을 것
	UPROPERTY(VisibleAnywhere, Category = "Effect|Camera|Anim")
	TObjectPtr<class UCurveVector> CameraAnimCurve;

	FTimerHandle CameraAnimTimer;
	UPROPERTY(EditAnywhere, Category = "Effect|Camera|Anim")
	float CameraAnimInterval{ 0.05f };
	UPROPERTY(VisibleAnywhere, Category = "Effect|Camera|Anim")
	float CameraAnimElapsedTime{ 0.0f };
	UPROPERTY(VisibleAnywhere, Category = "Effect|Camera|Anim")
	float CameraAnimDuration{ 1.0f };

#pragma endregion

public:	
	virtual void Tick(float DeltaTime) override;
	void Init();

#pragma region Input Action
	
	void SetInputDirection(FVector2D _dir)
	{
		InputDirection = _dir;
		InputDirection.Normalize();
	}
	const FVector2D& GetInputDirection() { return InputDirection; }
	void SetIsSprint(bool _bisSprint);
	bool GetIsSprint() const { return bIsSprint; }
	void SetIsCombat(bool _bIsCombat);
	bool GetIsCombat() const { return bIsCombat; }

	void Dodge();
	void Attack(EAttackType _eType);
	void AttackEnd();
	void SetActionProcess(EActionProcess _eProcess);

	void UseQuickSlot(uint8 _index);
	void HandleUseItemNotify();

	void Interact();
	void SetIgnoreInput(bool _bIgnoreMoveInput);

#pragma endregion

	// IAttackNotifyHandler을(를) 통해 상속됨
	void HandleAttackNotify(uint8 _opt) override;
	bool HitTarget(FHitResult& _hit, uint32 _damage, uint8 _opt);

	void ShakeCameraOnAttack(float _scale = 1.0f);
	void ShakeCamera(TSubclassOf<UCameraShakeBase> _shakeClass, float _scale = 1.0f);
	void SetCameraLag(bool _bIsEnable, float _speed = 0.0f);
	void PlayCameraAnim(TObjectPtr<UCurveVector> _animCurve, float _duration = 1.0f);
	void ProgressCameraCurve();

	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(static_cast<uint8>(ActorGroup)); }
	void ReportPlayerActionEvent(uint8 _actionType, float _range);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void OnDead() override;
	void OnCharacterHit() override;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	void InitStat(class UPlayerManager* _pm);
	void InitEquipment(class UPlayerManager* _pm);
	void InitAction(class UPlayerManager* _pm);
	void InitUI();

	void UpdateEquipment(EEquipmentType _type, TWeakObjectPtr<class UEquipmentItem> _equipment);

	void SmoothRotateToInputDir(float DeltaTime);
	void CheckInteractable();
};