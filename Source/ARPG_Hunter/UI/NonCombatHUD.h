// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PlayerHUD.h"
#include "NonCombatHUD.generated.h"

class UUWNonCombatHUD;
class UUWInventory;
class UUWEquipment;
class UUWMaintenance;

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
	TSubclassOf<UUWMaintenance> MaintenanceUIClass;
	UPROPERTY()
	TObjectPtr<UUWMaintenance> MaintenanceUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWInventory> InventoryUIClass;
	UPROPERTY()
	TObjectPtr<UUWInventory> InventoryUI;
	

protected:
	void BeginPlay() override;

public:
	void ShowMaintenanceUI();
	void HideMaintenanceUI();

	void ShowInventoryUI();
	void HideInventoryUI();
};
