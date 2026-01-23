// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/Enum.h"
#include "ActionComponent.generated.h"

struct FWeaponTypeData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	TObjectPtr<UAnimInstance> OwnerAnimInstance;
	
	TObjectPtr<FWeaponTypeData> CurWeaponType;
	uint8 CurAttackActionID{ 0 };
	bool bIsInAttackCombo{ false };

	bool bIsEnableNextAction{ true };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ActionResetSecond{ 1.5f };
	
	FTimerHandle ActionResetTimer;

	void SetActionResetTimer(float _second);
	bool IsValidAttackInput(EAttackType _type);

public:	
	UActionComponent();

	void BeginPlay() override;

	void Init(UAnimInstance* _ownerAnimInstance);
	void ResetAction();
	void SetEnableNextAction(bool _enable) { bIsEnableNextAction = _enable; }

	bool IsValid() { return CurWeaponType != nullptr; }

	void PlayDodgeAction(bool _isMoving, TFunction<bool(float)> _predicate);
	void PlayAttackAction(EAttackType _type, TFunction<bool(float)> _predicate);

	void PlayHitAction(bool _isDead);
};
