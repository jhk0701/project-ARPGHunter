// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombatHUD.h"

#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWMaintenanceWindow.h"
#include "UI/UserWidget/UWInventory.h"


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
		{
			NonCombatUI->AddToViewport();
			NonCombatUI->OnClickMaintenanceButton.BindLambda(
				[this]() 
				{
					ShowMaintenanceUI();
				}
			);
		}
	}

	if (MaintenanceUIClass)
	{
		MaintenanceUI = CreateWidget<UUWMaintenanceWindow>(GetWorld(), MaintenanceUIClass);
		if (MaintenanceUI) 
		{
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
			TObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
			TObjectPtr<UUWInventory> InventoryUI = MaintenanceUI->GetInventory();
			
			InventoryUI->Init(Inventory->GetContainer());
			Inventory->OnInventoryChanged.AddUObject(InventoryUI, &UUWInventory::SetSlot);
		}
	}
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
