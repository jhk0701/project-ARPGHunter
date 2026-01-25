// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Stat/StatComponent.h"
#include "MonsterStatComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARPG_HUNTER_API UMonsterStatComponent : public UStatComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Stat|Stagger", meta = (AllowPrivateAccess = "true"))
	uint16 MaxStagger{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stagger", meta = (AllowPrivateAccess = "true"))
	float StaggerRecoverTime{ 5.0f };

	UPROPERTY(VisibleAnywhere)
	uint16 Stagger{ 100 };

	FTimerHandle StaggerRecoverTimer;

public:
	FOnStatValueChanged OnStaggerChanged;

	void Init() override;

	bool IsStaggering(){ return Stagger == 0; }
	void TakeStaggerDamage(uint16 _staggerDamage);

	uint16 GetStagger() { return Stagger; }
	uint16 GetMaxStagger() { return MaxStagger; }

	void StartStaggerRecover();
	void RecoverStagger();
};
