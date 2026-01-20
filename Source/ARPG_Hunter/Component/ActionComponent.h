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
	FAction* LastAttackAction{ nullptr };
	uint8 AttackActionID[static_cast<uint8>(EAttackType::END)];

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ComboResetSecond{ 2.0f };
	FTimerHandle ComboResetTimer;

	void SetComboResetTimer(float _second);

public:	
	UActionComponent();

	void BeginPlay() override;

	void Init(UAnimInstance* _ownerAnimInstance);
	void ResetCombo();

	bool IsValid() { return CurWeaponType != nullptr; }
	bool Dodge(bool _isMoving, TFunction<bool(float)> _condition);
	bool Attack(EAttackType _type, TFunction<bool(float)> _condition);
};
