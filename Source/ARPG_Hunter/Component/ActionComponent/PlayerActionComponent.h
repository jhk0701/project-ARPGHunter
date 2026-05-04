// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ActionComponent/ActionComponent.h"
#include "PlayerActionComponent.generated.h"

class UWeaponConfig;
class UActionInstance;

enum class EActionProcess : uint8;
enum class EActionInput : uint8;
enum class EAttackType : uint8;
enum class EActionEvent : uint8;

USTRUCT()
struct FPlayerActionInitParam 
{
	GENERATED_BODY()
public:
	TWeakObjectPtr<UWeaponConfig> WeaonConfig;
	TWeakObjectPtr<UAnimInstance> OwnerAnimInstance;
	TWeakObjectPtr<USkeletalMeshComponent> FirePointComp;
	const TMap<uint8, TMap<uint8, int8>>* SkillDevelop;
};

USTRUCT()
struct FActionConnect
{
	GENERATED_BODY()
public:
	uint8 Index;
	bool bIsUnlocked;
};

USTRUCT()
struct FAppliedGraph 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<TObjectPtr<UActionInstance>> Actions;
	TArray<TMap<EAttackType, FActionConnect>> Graph;
	TMap<EAttackType, FActionConnect> GraphStart;
};

DECLARE_DELEGATE_ThreeParams(FOnActionUpdated, bool, int8, const FAppliedGraph*);
DECLARE_DELEGATE_RetVal_OneParam(bool, FStaminaUsagePredicate, uint32);

UCLASS()
class ARPG_HUNTER_API UPlayerActionComponent : public UActionComponent
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<UWeaponConfig> CurWeapon;

	UPROPERTY(VisibleAnywhere)
	FAppliedGraph AppliedGraph;

	int8 CurAttackActionID{ -1 };
	EActionProcess CurActionProcess;
	EActionInput CurActionInput;

	UPROPERTY(EditAnywhere)
	float ActionProgressRate{ 0.1f };
	FTimerHandle ActionProgressTimer;
	
	UPROPERTY(EditAnywhere)
	float ActionResetSecond{ 1.5f };
	FTimerHandle ActionResetTimer;

	bool IsValidAttackInput(EAttackType _type);
	bool IsInAttackCombo() const { return CurAttackActionID >= 0; } // bool bIsInAttackCombo{ false };
	void SetActionResetTimer(float _second);
	void ClearActionResetTimer();
	void ClearActionProgressTimer();
	void BroadcastActionUpdated();
	TObjectPtr<UAnimMontage> GetCurrentMontage();

protected:
	TWeakObjectPtr<class UAction> GetCurrentAction() const override;
	void PostProcessAttack(uint8 _opt, const TArray<FHitResult>& _inHitResults) override;

public:
	FOnActionUpdated OnActionUpdated;
	FStaminaUsagePredicate StaminaUsagePredicate;

	void Init(const FPlayerActionInitParam& _param);
	virtual void Clear() override;

	void ResetAction();
	void SetActionProcess(EActionProcess _eProcess);

	bool IsValid() const override { return CurWeapon.IsValid(); }
	bool IsInProgress() const;

	bool PlayDodgeAction(bool _isMoving);
	void PlayHitAction();
	void PlayDeadAction();
	void PlayItemUsageAction();
	bool PlayAttackAction(EAttackType _type);
	void ProcessAttackProgress();
	void ProcessAttackEnd();

	TObjectPtr<UAnimMontage> GetDodgeMontage() const;
	TObjectPtr<UAnimMontage> GetHitMontage() const;
	uint16 GetAttackActionDamagePer(uint8 _opt);
	uint16 GetAttackActionStaggerDamage(uint8 _opt);
	float GetAttackActionKnockBack(uint8 _opt);
	EAttackType GetAttackActionType();

	void ActivateActionInstanceEffect(EActionEvent _type, TObjectPtr<AActor> _target, TWeakObjectPtr<UActionInstance> _actionInst);
};