// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPopUp.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UUWPopUp : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	virtual void ShowUI();
	UFUNCTION()
	virtual void HideUI();
};
