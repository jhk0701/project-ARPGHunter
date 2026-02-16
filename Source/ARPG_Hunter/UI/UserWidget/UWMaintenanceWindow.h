// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWMaintenanceWindow.generated.h"

class UButton;
class UUWInventory;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWMaintenanceWindow : public UUWPopUp
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUWInventory> Inventory;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

protected:
	void NativeOnInitialized() override;

public:
	TObjectPtr<UUWInventory> GetInventory() const { return Inventory; }

	UFUNCTION()
	void ClickCloseButton();
	
};
