// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPlayerHUD.generated.h"

class UButton;
class UUWPlayerStatusBar;

DECLARE_DELEGATE(FOnClickButton)

/**
 * 상시 노출시킬 UI : 메뉴 버튼 등등
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UUWPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
};

UCLASS()
class ARPG_HUNTER_API UUWCombatHUD : public UUWPlayerHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWPlayerStatusBar> PlayerStatusBar;

public:
	TObjectPtr<UUWPlayerStatusBar> GetPlayerStatusBar() const { return PlayerStatusBar; }
};

UCLASS()
class ARPG_HUNTER_API UUWNonCombatHUD : public UUWPlayerHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MaintenanceButton;

protected:
	void NativeOnInitialized() override;

public:
	FOnClickButton OnClickMaintenanceButton;
	
	UFUNCTION()
	void ClickMaintenanceButton();
};