// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWDamageFont.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWDamageFont : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> DamageLabel;
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> MoveAnim;

	UPROPERTY(EditAnywhere, Category = "Color")
	FColor ColorOnNormal;
	UPROPERTY(EditAnywhere, Category = "Color")
	FColor ColorOnCritical;
	
public:
	void SetDamage(uint32 _amount, bool _bIsCritical);
	void PlayAnim();
};