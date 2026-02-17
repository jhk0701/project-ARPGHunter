// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PlayerHUD.h"
#include "NonCombatHUD.generated.h"

class UUWNonCombatHUD;
class UUWInventory;
class UUWEquipment;
class UUWMaintenanceWindow;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ANonCombatHUD : public APlayerHUD
{
	GENERATED_BODY()
public:
	ANonCombatHUD();
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWNonCombatHUD> NonCombatUIClass;
	UPROPERTY()
	TObjectPtr<UUWNonCombatHUD> NonCombatUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWInventory> InventoryUIClass;
	UPROPERTY()
	TObjectPtr<UUWInventory> InventoryUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWEquipment> EquipmentUIClass;
	UPROPERTY()
	TObjectPtr<UUWEquipment> EquipmentUI;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWMaintenanceWindow> MaintenanceUIClass;
	UPROPERTY()
	TObjectPtr<UUWMaintenanceWindow> MaintenanceUI;

protected:
	void BeginPlay() override;

public:
	TObjectPtr<UUWMaintenanceWindow> GetMaintenanceUI() const { return MaintenanceUI; }
	void ShowMaintenanceUI();
	void HideMaintenanceUI();
};
