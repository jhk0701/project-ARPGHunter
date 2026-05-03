// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/Effectable.h"
#include "Interface/AttackNotifyHandler.h"
#include "ARPGCharacterBase.generated.h"

UCLASS(Abstract)
class ARPG_HUNTER_API AARPGCharacterBase : 
	public ACharacter, 
	public IHitable, 
	public IEffectable, 
	public IAttackNotifyHandler
{
	GENERATED_BODY()

public:
	AARPGCharacterBase();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStatComponent> StatComp;

public:
	// Inherited via IEffectable
	TWeakObjectPtr<class UStatComponent> GetStatComp() const override { return StatComp; };
	void ApplyEffect(const FApplyEffectParam& _param) override;

	bool IsDead() const;

	// Inherited via IAttackNotifyHandler
	virtual void HandleAttackNotify(uint8 _opt) override;

	// Inherited via IHitable
	virtual void HitBy(const FHitInfo& _hitInfo) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnDead() {};
};
