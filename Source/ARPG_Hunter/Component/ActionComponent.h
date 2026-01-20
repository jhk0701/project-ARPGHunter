// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/Define.h"
#include "ActionComponent.generated.h"

struct FWeaponTypeData;
struct FAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UAnimInstance* OwnerAnimInstance;

	FWeaponTypeData* CurWeaponType;

	EAttackType LastAttackType;
	uint8 AttackActionID[static_cast<uint8>(EAttackType::END)];
	bool IsEnableNextAction{ true };

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ActionResetSecond{ 2.0f };
	
	FTimerHandle ActionResetTimer;

	void SetActionResetTimer(float _second);
	bool IsValidAttackInput(EAttackType _type);
	const uint8 GetActionID(EAttackType _type) { return AttackActionID[static_cast<uint8>(_type)]; }

public:	
	UActionComponent();

	void BeginPlay() override;

	void Init(UAnimInstance* _ownerAnimInstance);
	void ResetAction();

	bool IsValid() { return CurWeaponType != nullptr; }
	void Dodge(bool _isMoving, TFunction<bool(float)> _predicate);
	void Attack(EAttackType _type, TFunction<bool(float)> _predicate);

	void SetEnableNextAction(bool _enable) { IsEnableNextAction = _enable; }
};
