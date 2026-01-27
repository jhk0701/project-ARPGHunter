// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/Effect.h"
#include "RepeatedActivateEffect.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API URepeatedActivateEffect : public UEffect
{
	GENERATED_BODY()
private:
	FTimerHandle RepeatTimer;

public:
	void Activate(TWeakObjectPtr<UStatComponent> _target, FEffectParam* _param) final;
	virtual void Deactivate() override;
	
	virtual void RepeatedActivate();
};
