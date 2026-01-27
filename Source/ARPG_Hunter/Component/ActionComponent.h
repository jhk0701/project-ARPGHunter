// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/Enum.h"
#include "ActionComponent.generated.h"

struct FWeaponTypeData;
struct FActionRange;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TObjectPtr<UAnimInstance> OwnerAnimInstance;
	
	TObjectPtr<FWeaponTypeData> CurWeaponType;
	bool bIsInAttackCombo{ false };

	uint8 CurAttackActionID{ 0 };
	EActionProcess CurActionProcess{ EActionProcess::NONE };
	EActionInput CurActionInput{EActionInput::NORMAL};
	TObjectPtr<UAnimMontage> CurActionMontage{nullptr};

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ActionResetSecond{ 1.0f };
	
	FTimerHandle ActionResetTimer;

	void SetActionResetTimer(float _second);
	bool IsValidAttackInput(EAttackType _type);

	void ActivateActionEffect(const TArray<TObjectPtr<class UEffectData>>& _effectArray, TObjectPtr<AActor> _target);

public:	
	UActionComponent();

	void BeginPlay() override;

	void Init(UAnimInstance* _ownerAnimInstance);
	void ResetAction();
	void SetActionProcess(EActionProcess _eProcess);

	bool IsValid() { return CurWeaponType != nullptr; }
	bool IsInProgress() { return CurActionProcess == EActionProcess::IN_PROGRESS; }

	void PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate);
	void PlayHitAction(bool _isDead);

	void PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate);
	void ProcessAttackEnd();

	uint16 GetAttackActionDamagePer();
	uint16 GetAttackActionStaggerDamage();
	
	bool TraceAttack(uint8 _opt, TArray<FHitResult>& _outHitResult);
};
