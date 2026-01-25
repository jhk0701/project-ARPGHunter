// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatValueChanged, uint16, uint16)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 MaxHealth{ 100 };
	// UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	// uint16 Strength{ 10 };
	// UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	// uint16 Agility{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 Attack{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 Defense{ 10 };

	UPROPERTY(VisibleAnywhere)
	uint16 Health{ 100 };

public:	
	FOnStatValueChanged OnHealthChanged;

	virtual void Init();
	virtual void TakeDamage(uint16 _damage);
	void RecoverHealth(uint16 _amount);

	bool IsDead() { return Health == 0; }
	uint16 GetHealth() { return Health; }
	uint16 GetMaxHealth() { return MaxHealth; }

	uint16 GetAttack() { return Attack; }
};
