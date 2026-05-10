// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/PlayerHUD.h"
#include "NonCombatHUD.generated.h"

class UUWNonCombatHUD;
class UUWMaintenance;
class UUWInventory;
class UUWSkillDevelop;

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
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWSkillDevelop> SkillDevelopUIClass;
	UPROPERTY()
	TObjectPtr<UUWSkillDevelop> SkillDevelopUI;

public:
	void ToggleMaintenanceUI();
	void ToggleInventoryUI();
	void ToggleSkillDevelopUI();

protected:
	void BeginPlay() override;
	void InitMenuUI() override;

private:
	void InitNonCombatHUD();
	void InitMaintenanceUI(class UPlayerManager* _pm);
	void InitInventoryUI(class UPlayerManager* _pm);
	void InitSkillDevelopUI(class UPlayerManager* _pm);

	void BindMainenanceAndInventory();
};
