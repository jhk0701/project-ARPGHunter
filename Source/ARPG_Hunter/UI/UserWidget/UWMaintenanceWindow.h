// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWMaintenanceWindow.generated.h"

class UButton;
class UNamedSlot;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWMaintenanceWindow : public UUWPopUp
{
	GENERATED_BODY()
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNamedSlot> InventorySlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UNamedSlot> EquipmentSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

protected:
	void NativeOnInitialized() override;

public:
	void SetInventoryUI(TObjectPtr<UUserWidget> _ui);
	void SetEquipmentUI(TObjectPtr<UUserWidget> _ui);
	
	UFUNCTION()
	void ClickCloseButton();
};
