// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombatHUD.h"

#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWMaintenanceWindow.h"
#include "UI/UserWidget/UWInventory.h"
#include "UI/UserWidget/UWEquipment.h"


ANonCombatHUD::ANonCombatHUD()
{
	static ConstructorHelpers::FClassFinder<UUWNonCombatHUD> NonCombatUIFinder(TEXT("/Game/06-UI/HUD/WBP_NonCombatHUD.WBP_NonCombatHUD_C"));
	if (NonCombatUIFinder.Succeeded())
		NonCombatUIClass = NonCombatUIFinder.Class;

	static ConstructorHelpers::FClassFinder<UUWInventory> InventoryUIFinder(TEXT("/Game/06-UI/WBP_Inventory.WBP_Inventory_C"));
	if (InventoryUIFinder.Succeeded())
		InventoryUIClass = InventoryUIFinder.Class;
	
	static ConstructorHelpers::FClassFinder<UUWEquipment> EquipmentUIFinder(TEXT("/Game/06-UI/WBP_Equipment.WBP_Equipment_C"));
	if (EquipmentUIFinder.Succeeded())
		EquipmentUIClass = EquipmentUIFinder.Class;

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

	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (InventoryUIClass)
	{
		InventoryUI = CreateWidget<UUWInventory>(GetWorld(), InventoryUIClass);
		if (InventoryUI) 
		{
			TObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
			InventoryUI->Init(Inventory->GetContainer(), PlayerManager->GetGold());
			Inventory->OnInventoryChanged.AddUObject(InventoryUI, &UUWInventory::SetSlot);
			PlayerManager->GetGoldChangedEvent().AddUObject(InventoryUI, &UUWInventory::SetGoldLabel);
		}
	}

	if (EquipmentUIClass)
	{
		EquipmentUI = CreateWidget<UUWEquipment>(GetWorld(), EquipmentUIClass);
		if (EquipmentUI) 
		{
			TObjectPtr<UEquipment> Equipment = PlayerManager->GetEquipment();
			EquipmentUI->Init(Equipment->GetContainer(), Equipment->GetEquipmentStat());
			Equipment->OnEquipmentChanged.AddUObject(EquipmentUI, &UUWEquipment::SetSlot);
			Equipment->OnStatValueChanged.AddUObject(EquipmentUI, &UUWEquipment::SetStatInfo);
		}
	}

	if (MaintenanceUIClass)
		MaintenanceUI = CreateWidget<UUWMaintenanceWindow>(GetWorld(), MaintenanceUIClass);
}

void ANonCombatHUD::ShowMaintenanceUI()
{
	if (nullptr == MaintenanceUI)
		return;

	MaintenanceUI->SetInventoryUI(InventoryUI);
	MaintenanceUI->SetEquipmentUI(EquipmentUI);
	MaintenanceUI->ShowUI();
}

void ANonCombatHUD::HideMaintenanceUI()
{
	if (nullptr == MaintenanceUI)
		return;

	MaintenanceUI->ClickCloseButton();
}
