// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWMaintenanceWindow.generated.h"

class UButton;
class UUWInventory;
class UUWEquipment;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWMaintenanceWindow : public UUWPopUp
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWInventory> Inventory;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWEquipment> Equipment;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

protected:
	void NativeOnInitialized() override;

public:
	const TObjectPtr<UUWInventory> GetInventory() const { return Inventory; }
	const TObjectPtr<UUWEquipment> GetEquipment() const { return Equipment; }

	UFUNCTION()
	void ClickCloseButton();
	
};
