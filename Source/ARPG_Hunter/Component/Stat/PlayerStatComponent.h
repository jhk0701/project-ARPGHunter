// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Stat/StatComponent.h"
#include "PlayerStatComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARPG_HUNTER_API UPlayerStatComponent : public UStatComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	uint32 MaxStamina{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	uint32 StaminaRecoveryPerSecond{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryRate{ 0.1f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryPauseTime{ 1.0f };
	UPROPERTY(VisibleAnywhere)
	uint16 Stamina{ 100 };

	FTimerHandle StaminaRecoveryTimer;
	void StartStaminaRecovery();

public:
	FOnStatValueChanged OnStaminaChanged;
	void Init() override;

	uint16 GetStamina() { return Stamina; }
	uint16 GetMaxStamina() { return MaxStamina; }

	bool TryUseStamina(uint16 _amount);
	void RecoverStamina(uint16 _amount);

	void PauseAndRestartStaminaRecovery(float _pauseSecond);
};
