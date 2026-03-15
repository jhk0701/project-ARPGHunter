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
	TMap<EAttackType, FActionConnect> GraphStart;
	TArray<TMap<EAttackType, FActionConnect>> AppliedGraph;

	bool bIsInAttackCombo{ false };
	int16 CurAttackActionID{ -1 };
	EActionProcess CurActionProcess;
	EActionInput CurActionInput;
	TFunction<bool(float)> CurActionPredicate{ nullptr };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ActionProgressRate{ 0.1f };
	FTimerHandle ActionProgressTimer;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ActionResetSecond{ 1.5f };
	FTimerHandle ActionResetTimer;

	bool IsValidAttackInput(EAttackType _type);
	void SetActionResetTimer(float _second);
	void ClearActionResetTimer();
	void ClearActionProgressTimer();
	void BroadcastActionUpdated();
	TObjectPtr<UAnimMontage> GetCurrentMontage();

public:
	FOnActionUpdated OnActionUpdated;

	void Init(TObjectPtr<UWeaponConfig> _data, TWeakObjectPtr<UAnimInstance> _ownerAnimInstance, TWeakObjectPtr<USkeletalMeshComponent> _firePointComp);
	virtual void Clear() override;
	virtual void ProcessAttack(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction, TWeakObjectPtr<AActor> _target = nullptr) override;

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

	TObjectPtr<UAnimMontage> GetDodgeMontage() const;
	TObjectPtr<UAnimMontage> GetHitMontage() const;
	uint16 GetAttackActionDamagePer(uint8 _opt);
	uint16 GetAttackActionStaggerDamage(uint8 _opt);
	float GetAttackActionKnockBack(uint8 _opt);
	EAttackType GetAttackActionType();
};