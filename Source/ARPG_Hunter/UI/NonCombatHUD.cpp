// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombatHUD.h"

#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWMaintenanceWindow.h"


ANonCombatHUD::ANonCombatHUD()
{
	static ConstructorHelpers::FClassFinder<UUWNonCombatHUD> NonCombatUIFinder(TEXT("/Game/06-UI/HUD/WBP_NonCombatHUD.WBP_NonCombatHUD_C"));
	if (NonCombatUIFinder.Succeeded())
		NonCombatUIClass = NonCombatUIFinder.Class;

	static ConstructorHelpers::FClassFinder<UUWMaintenanceWindow> MaintenanceUIFinder(TEXT("/Game/06-UI/WBP_MaintenanceWindow.WBP_MaintenanceWindow_C"));
	if (MaintenanceUIFinder.Succeeded())
		MaintenanceUIClass = MaintenanceUIFinder.Class;
}

void ANonCombatHUD::BeginPlay()
{
	Super::BeginPlay();

	if (NonCombatUIClass) 
	{
		NonCombatUI = CreateWidget<UUWNonCombatHUD>(GetWorld(), NonCombatUIClass);
		if (NonCombatUI)
			NonCombatUI->AddToViewport();
	}

	if (MaintenanceUIClass) 
		MaintenanceUI = CreateWidget<UUWMaintenanceWindow>(GetWorld(), MaintenanceUIClass);
}

void ANonCombatHUD::ShowMaintenanceUI()
{
	if (nullptr == MaintenanceUI)
		return;

	MaintenanceUI->ShowUI();
}

void ANonCombatHUD::HideMaintenanceUI()
{
	if (nullptr == MaintenanceUI)
		return;

	MaintenanceUI->HideUI();
}
