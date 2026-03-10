// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:
	APlayerHUD();

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUWPopUp> InputGuideUIClass;
	UPROPERTY()
	TObjectPtr<UUWPopUp> InputGuideUI;

protected:
	void BeginPlay() override;

public:
	void ToggleInputGuideUI();
};
