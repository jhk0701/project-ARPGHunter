// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPlayerHUD.generated.h"

class UUWPlayerStatusBar;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWPlayerStatusBar> PlayerStatusBar;

public:
	UUWPlayerStatusBar* GetPlayerStatusBar() const { return PlayerStatusBar; }
};
