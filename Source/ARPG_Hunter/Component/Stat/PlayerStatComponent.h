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
	uint16 MaxStamina{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	uint16 StaminaRecoveryPerSecond{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryRate{ 0.1f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryPauseTime{ 1.0f };
	UPROPERTY(VisibleAnywhere, Category = "Stat|Resource")
	uint16 Stamina{ 100 };

	UPROPERTY(EditAnywhere, Category = "Stat|Skill", meta = (AllowPrivateAccess = "true"))
	uint8 MaxSkill{ 100 };
	UPROPERTY(VisibleAnywhere, Category = "Stat|Resource")
	uint8 Skill{ 0 };

	FTimerHandle StaminaRecoveryTimer;
	void StartStaminaRecovery();

public:
	FOnStatValueChanged OnStaminaChanged;
	FOnStatValueChanged OnSkillChanged;

	void Init() override;

	uint16 GetMaxStamina() { return MaxStamina; }
	uint16 GetStamina() { return Stamina; }
	bool TryUseStamina(uint16 _amount);
	void RecoverStamina(uint16 _amount);
	void PauseAndRestartStaminaRecovery(float _pauseSecond);

	uint8 GetMaxSkill() { return MaxSkill; }
	uint8 GetSkill() { return Skill; }
	bool TryUseSkill(uint8 _amount);
	void RecoverSkill(uint8 _amount);
};
