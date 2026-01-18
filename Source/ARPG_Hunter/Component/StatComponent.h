// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatValueChanged, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 MaxHealth{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 MaxStamina{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 StaminaRecovery{ 10 };
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
	UPROPERTY(VisibleAnywhere)
	uint16 Stamina{ 100 };

	FTimerHandle StaminaRecoveryTimerHandle;

protected:
	virtual void BeginPlay() override;

public:	
	FOnStatValueChanged OnTakeDamage;
	FOnStatValueChanged OnUseStamina;

	void TakeDamage(uint16 _damage);
	bool TryUseStamina(uint16 _amount);

	float GetHealthPercent() { return static_cast<float>(Health) / MaxHealth; }
	float GetStaminaPercent() { return static_cast<float>(Stamina) / MaxStamina; }
	bool IsDead() { return Health == 0; }
};
