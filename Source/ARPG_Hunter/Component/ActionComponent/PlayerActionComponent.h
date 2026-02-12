// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ActionComponent/ActionComponent.h"
#include "PlayerActionComponent.generated.h"

class UAction;
struct FWeaponTypeData;
enum class EActionProcess : uint8;
enum class EActionInput : uint8;
enum class EAttackType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UPlayerActionComponent : public UActionComponent
{
	GENERATED_BODY()

private:
	FWeaponTypeData* CurWeaponType;
	bool bIsInAttackCombo{ false };
	
	uint8 CurAttackActionID{ 0 };
	EActionProcess CurActionProcess;
	EActionInput CurActionInput;
	TFunction<bool(float)> CurActionPredicate{ nullptr };
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ActionProgressRate{ 0.1f };
	FTimerHandle ActionProgressTimer;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ActionResetSecond{ 1.5f };
	FTimerHandle ActionResetTimer;
	
	void SetActionResetTimer(float _second);
	bool IsValidAttackInput(EAttackType _type);

	void ClearActionProgressTimer();

public:
	void Init(FTableRowBase* _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp) override;
	void Clear() override;

	void ResetAction();
	void SetActionProcess(EActionProcess _eProcess);

	bool IsValid() const { return CurWeaponType != nullptr; }
	bool IsInProgress() const;

	void PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate);
	void PlayHitAction(bool _isDead);

	void PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate);
	void ProcessAttackProgress();
	void ProcessAttackEnd();

	uint16 GetAttackActionDamagePer();
	uint16 GetAttackActionStaggerDamage();
	float GetAttackActionKnockBack(uint8 _opt);
	EAttackType GetAttackActionType();
};