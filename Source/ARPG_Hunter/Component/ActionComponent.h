// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

struct FWeaponTypeData;
struct FAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	FWeaponTypeData* CurWeaponType;

	uint8 NormalAttack{0};
	uint8 SmashAttack{0};

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float InitSecond{1.0f};

	FTimerHandle InitAttackActionTimer;

public:	
	UActionComponent();

	void BeginPlay() override;

	void Init();
	void InitAttackAction();

	bool IsValid() { return CurWeaponType != nullptr; }
	const FAction& GetDodgeAction();

	const FAction& GetAttackAction(bool _isNormalAttack);
};
