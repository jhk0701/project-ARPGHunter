// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPlayerHUD.generated.h"

class UButton;
class UUWPlayerStatusBar;
class UUWQuickSlot;

enum class EShortCutType : uint8;

DECLARE_DELEGATE_OneParam(FOnClickButton, EShortCutType)

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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWQuickSlot> QuickSlot;

public:
	TObjectPtr<UUWPlayerStatusBar> GetPlayerStatusBar() const { return PlayerStatusBar; }
	TObjectPtr<UUWQuickSlot> GetQuickSlot() const { return QuickSlot; }
};

UCLASS()
class ARPG_HUNTER_API UUWNonCombatHUD : public UUWPlayerHUD
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MaintenanceButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> InventoryButton;

protected:
	void NativeOnInitialized() override;

public:
	FOnClickButton OnClickShortCutButton;
	
	UFUNCTION()
	void ClickMaintenanceButton();
	UFUNCTION()
	void ClickInventoryButton();
};