// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombatHUD.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWMaintenance.h"
#include "UI/UserWidget/UWInventory.h"


ANonCombatHUD::ANonCombatHUD()
{
	static ConstructorHelpers::FClassFinder<UUWNonCombatHUD> NonCombatUIFinder(TEXT("/Game/06-UI/HUD/WBP_NonCombatHUD.WBP_NonCombatHUD_C"));
	if (NonCombatUIFinder.Succeeded())
		NonCombatUIClass = NonCombatUIFinder.Class;
	static ConstructorHelpers::FClassFinder<UUWMaintenance> MaintenanceUIFinder(TEXT("/Game/06-UI/WBP_Maintenance.WBP_Maintenance_C"));
	if (MaintenanceUIFinder.Succeeded())
		MaintenanceUIClass = MaintenanceUIFinder.Class;
	static ConstructorHelpers::FClassFinder<UUWInventory> InventoryUIFinder(TEXT("/Game/06-UI/WBP_Inventory.WBP_Inventory_C"));
	if (InventoryUIFinder.Succeeded())
		InventoryUIClass = InventoryUIFinder.Class;
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
			NonCombatUI->OnClickShortCutButton.BindLambda(
				[this](EShortCutType _type)
				{
					switch (_type)
					{
						/*case EShortCutType::ESC:
							break;*/
					case EShortCutType::TAB:
						ShowMaintenanceUI();
						break;
					case EShortCutType::INVENTORY:
						ShowInventoryUI();
						break;
					}
					
				}
			);
		}
	}

	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (MaintenanceUIClass)
	{
		MaintenanceUI = CreateWidget<UUWMaintenance>(GetWorld(), MaintenanceUIClass);

		TObjectPtr<UEquipment> Equipment = PlayerManager->GetEquipment();
		MaintenanceUI->Init(PlayerManager->GetStat(), PlayerManager->GetEquipmentStat(), Equipment->GetContainer());
		Equipment->OnEquipmentChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetEquipment);
		PlayerManager->OnStatValueChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetStatInfo);
	}

	if (InventoryUIClass)
	{
		InventoryUI = CreateWidget<UUWInventory>(GetWorld(), InventoryUIClass);
		if (InventoryUI)
		{
			TObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
			
			InventoryUI->Init(
				Inventory->GetContainerSize(), 
				PlayerManager->GetGold(), 
				[Inventory](EItemType _type)
				{ 
					return &Inventory->GetContainer(_type);
				}
			);
			Inventory->OnInventoryChanged.AddUObject(InventoryUI, &UUWInventory::SetSlot);
			PlayerManager->GetGoldChangedEvent().AddUObject(InventoryUI, &UUWInventory::SetGoldLabel);
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

void ANonCombatHUD::ShowInventoryUI()
{
	if (nullptr == InventoryUI)
		return;

	InventoryUI->ShowUI();
}

void ANonCombatHUD::HideInventoryUI()
{
	if (nullptr == InventoryUI)
		return;

	InventoryUI->HideUI();
}
