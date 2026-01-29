// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageFont.generated.h"

UCLASS()
class ARPG_HUNTER_API ADamageFont : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageFont();

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> WidgetComp;

	FTimerHandle ShowTimer;

public:
	void UpdateUI(uint32 _damage, bool _bIsCritial);
	void ShowUI(float _duration = 3.0f);
	void HideUI();
};
