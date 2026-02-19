// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombatHUD.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"
#include "Player/QuickSlot.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWMaintenance.h"
#include "UI/UserWidget/UWInventory.h"
#include "Item/Item.h"
#include "Data/ItemData.h"

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
		if (MaintenanceUI) 
		{
			TObjectPtr<UEquipment> Equipment = PlayerManager->GetEquipment();
			TObjectPtr<UQuickSlot> QuickSlot = PlayerManager->GetQuickSlot();

			FUWMaintenanceInitParam InitParm
			(
				PlayerManager->GetStat(),
				PlayerManager->GetEquipmentStat(),
				Equipment->GetContainer(),
				QuickSlot->GetContainer()
			);

			MaintenanceUI->Init(InitParm);
			Equipment->OnEquipmentChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetEquipment);
			PlayerManager->OnStatValueChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetStatInfo);
		}
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

	if (MaintenanceUI && InventoryUI) 
	{
		MaintenanceUI->OnEquipmentSlotClicked.BindLambda(
			[this](EItemType _type, uint8 _opt) 
			{
				TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

				TObjectPtr<UItem> ItemToCompare;
				switch (_type)
				{
				case EItemType::CONSUMABLE:
					// 퀵슬롯 관련
					break;
				case EItemType::WEAPON:
					ItemToCompare = PlayerManager->GetEquipment()->GetEquipment(EEquipmentType::WEAPON);
					break;
				case EItemType::ARMOR:
					EEquipmentType Type = static_cast<EEquipmentType>(_opt);
					ItemToCompare = PlayerManager->GetEquipment()->GetEquipment(Type);
					break;
				}
				
				InventoryUI->ShowUI(_type, ItemToCompare);
			}
		);

		InventoryUI->OnThrowButtonClicked.BindLambda(
			[this](EItemType _type, uint8 _index) 
			{
				TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
				TObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
				Inventory->TrySubItem(_type, _index, Inventory->GetItem(_type, _index)->GetAmount());
			}
		);

		InventoryUI->OnEquipButtonClicked.BindLambda(
			[this](EItemType _type, uint8 _index) 
			{
				TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
				TObjectPtr<UItem> Item = PlayerManager->GetInventory()->GetItem(_type, _index);
				TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(Item->GetConfig());

				PlayerManager->GetEquipment()->Equip(EquipmentConfig->Type, Item); // 장착
			}
		);

		InventoryUI->OnUnequipButtonClicked.BindLambda(
			[this](EItemType _type, uint8 _index) 
			{
				TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
				TObjectPtr<UItem> Item = PlayerManager->GetInventory()->GetItem(_type, _index);
				TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(Item->GetConfig());

				PlayerManager->GetEquipment()->Unequip(EquipmentConfig->Type); // 장착 해제
			}
		);
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
