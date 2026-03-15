// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ActionComponent/ActionComponent.h"
#include "PlayerActionComponent.generated.h"

class UAction;
class UWeaponConfig;

enum class EActionProcess : uint8;
enum class EActionInput : uint8;
enum class EAttackType : uint8;

DECLARE_DELEGATE_ThreeParams(FOnActionUpdated, bool, uint8, TWeakObjectPtr<class UActionComboData>);

USTRUCT()
struct FActionConnect
{
	GENERATED_BODY()
public:
	uint8 Index;
	bool bIsConnected;
};

USTRUCT()
struct FAppliedAction 
{
	GENERATED_BODY()
public:
	TObjectPtr<UAction> Action;
	// 스킬 반영 수치
};


UCLASS()
class ARPG_HUNTER_API UPlayerActionComponent : public UActionComponent
{
	GENERATED_BODY()

private:
	TObjectPtr<UWeaponConfig> CurWeapon;
	TArray<FAppliedAction> AppliedActions;
	TArray<TMap<EAttackType, FActionConnect>> AppliedGraph;

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
	void ClearActionResetTimer();
	bool IsValidAttackInput(EAttackType _type);

	void ClearActionProgressTimer();

protected:
	virtual void SetCurrentAction(TObjectPtr<UAction> _action) override;

public:
	FOnActionUpdated OnActionUpdated;

	void Init(TObjectPtr<UWeaponConfig> _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp);
	void Clear() override;

	void ResetAction();
	void SetActionProcess(EActionProcess _eProcess);

	bool IsValid() const { return CurWeapon != nullptr; }
	TWeakObjectPtr<UWeaponConfig> GetWeaponConfig() const;
	bool IsInProgress() const;

	bool PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate);
	void PlayHitAction();
	void PlayDeadAction();
	void PlayItemUsageAction();

	bool PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate);
	void ProcessAttackProgress();
	void ProcessAttackEnd();

	UAnimMontage* GetDodgeMontage() const;
	UAnimMontage* GetHitMontage() const;
	uint16 GetAttackActionDamagePer(uint8 _opt);
	uint16 GetAttackActionStaggerDamage(uint8 _opt);
	float GetAttackActionKnockBack(uint8 _opt);
	EAttackType GetAttackActionType();
};