// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/NonCombatHUD.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"
#include "Player/QuickSlot.h"
#include "Player/SkillDevelop.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "UI/UserWidget/UWGameMenu.h"
#include "UI/UserWidget/UWMaintenance.h"
#include "UI/UserWidget/UWInventory.h"
#include "UI/UserWidget/UWSkillDevelop.h"

#include "Item/Item.h"

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
	static ConstructorHelpers::FClassFinder<UUWSkillDevelop> SkillUIFinder(TEXT("/Game/06-UI/WBP_SkillDevelop.WBP_SkillDevelop_C"));
	if (SkillUIFinder.Succeeded())
		SkillDevelopUIClass = SkillUIFinder.Class;
}

void ANonCombatHUD::BeginPlay()
{
	Super::BeginPlay();

	InitNonCombatHUD();
	
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	InitMaintenanceUI(PlayerManager);
	InitInventoryUI(PlayerManager);
	InitSkillDevelopUI(PlayerManager);
	BindMainenanceAndInventory();
}

void ANonCombatHUD::InitMenuUI()
{
	Super::InitMenuUI();

	TWeakObjectPtr<UUWPopUp> MenuUIInst = GetGameMenuUI();
	if (false == MenuUIInst.IsValid())
		return;

	TObjectPtr<UUWGameMenu> MenuUI = Cast<UUWGameMenu>(MenuUIInst);
	MenuUI->ToggleOptionalButton(UUWGameMenu::RETURN, false);
}

void ANonCombatHUD::InitNonCombatHUD()
{
	if (false == TryCreateWidget<UUWNonCombatHUD>(NonCombatUIClass, NonCombatUI))
		return;

	NonCombatUI->OnClickShortCutButton.BindLambda(
		[this](EShortCutType _type)
		{
			switch (_type)
			{
			case EShortCutType::TAB:
				ToggleMaintenanceUI();
				break;
			case EShortCutType::KEY_I:
				ToggleInventoryUI();
				break;
			case EShortCutType::KEY_K:
				ToggleSkillDevelopUI();
				break;
			}
		}
	);

	NonCombatUI->AddToViewport();
}

void ANonCombatHUD::InitMaintenanceUI(UPlayerManager* _pm)
{
	if (false == TryCreateWidget<UUWMaintenance>(MaintenanceUIClass, MaintenanceUI))
		return;

	TWeakObjectPtr<UEquipment> Equipment = _pm->GetEquipment();
	TWeakObjectPtr<UQuickSlot> QuickSlot = _pm->GetQuickSlot();

	FUWMaintenanceInitParam InitParm
	(
		_pm->GetLevel(),
		_pm->GetExp(),
		_pm->GetRequiredExp(),
		_pm->GetStat(),
		_pm->GetEquipmentStat(),
		Equipment->GetContainer(),
		QuickSlot->GetContainer()
	);

	MaintenanceUI->Init(InitParm);
	Equipment->OnEquipmentChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetEquipment);
	_pm->OnStatValueChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetStatInfo);
	_pm->OnExpChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetLevelInfo);
	QuickSlot->OnQuickSlotChanged.AddUObject(MaintenanceUI, &UUWMaintenance::SetQuickSlot);
	_pm->GetInventory()->OnInventoryChanged.AddUObject(MaintenanceUI, &UUWMaintenance::OnInventoryChanged);
}

void ANonCombatHUD::InitInventoryUI(UPlayerManager* _pm)
{
	if (false == TryCreateWidget<UUWInventory>(InventoryUIClass, InventoryUI))
		return;

	TWeakObjectPtr<UInventory> Inventory = _pm->GetInventory();
	InventoryUI->Init(
		Inventory->GetContainerSize(),
		_pm->GetGold(),
		FGetItemArrFunc::CreateUObject(Inventory.Get(), &UInventory::GetContainer)
	);
	Inventory->OnInventoryChanged.AddUObject(InventoryUI, &UUWInventory::SetSlot);
	_pm->GetGoldChangedEvent().AddUObject(InventoryUI, &UUWInventory::SetGoldLabel);
}

void ANonCombatHUD::InitSkillDevelopUI(UPlayerManager* _pm)
{
	if (false == TryCreateWidget<UUWSkillDevelop>(SkillDevelopUIClass, SkillDevelopUI))
		return;

	TWeakObjectPtr<USkillDevelop> SkillDevelop = _pm->GetSkillDevelop();
	SkillDevelopUI->Init(
		_pm->GetWeaponConfig(),
		FGetSkillUpgradeInfoFunc::CreateUObject(SkillDevelop.Get(), &USkillDevelop::GetSpecificSkillUpgrade),
		FGetUsableSkillPointFunc::CreateUObject(SkillDevelop.Get(), &USkillDevelop::GetUsableSkillPoint)
	);
	SkillDevelopUI->OnUpgradeClicked.BindWeakLambda(this,
		[SkillDevelop](uint8 _key, uint8 _nodeIdx, uint8 _upgradeIdx, uint8 _cost)
		{
			if (false == SkillDevelop.IsValid())
				return;

			if (SkillDevelop->TryUseSkillPoint(_cost))
				SkillDevelop->AddSkill(_key, _nodeIdx, _upgradeIdx);
		}
	);
}

void ANonCombatHUD::BindMainenanceAndInventory()
{
	if (nullptr == MaintenanceUI || nullptr == InventoryUI)
		return;

	MaintenanceUI->OnEquipmentSlotClicked.BindLambda(
		[this](EItemType _type, uint8 _opt)
		{
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

			TWeakObjectPtr<UItem> ItemToCompare;
			if (_type == EItemType::WEAPON)
				ItemToCompare = PlayerManager->GetEquipment()->GetEquipment(EEquipmentType::WEAPON);
			else if (_type == EItemType::ARMOR)
			{
				EEquipmentType Type = static_cast<EEquipmentType>(_opt);
				ItemToCompare = PlayerManager->GetEquipment()->GetEquipment(Type);
			}

			InventoryUI->SetSelectOption(_type, ItemToCompare, _opt);
			InventoryUI->ShowUI(true, MaintenanceUI);
		}
	);

	MaintenanceUI->OnQuickSlotClicked.BindLambda(
		[this](uint8 _index)
		{
			// 소비템 인벤토리 열기
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

			TWeakObjectPtr<UItem> ItemToCompare = PlayerManager->GetQuickSlot()->GetItem(_index);
			InventoryUI->SetSelectOption(EItemType::CONSUMABLE, ItemToCompare, _index);
			InventoryUI->ShowUI(true, MaintenanceUI);
		}
	);

	InventoryUI->OnThrowButtonClicked.BindLambda(
		[this](EItemType _type, uint8 _index)
		{
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
			TWeakObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
			Inventory->TrySubItem(_type, _index, Inventory->GetItem(_type, _index)->GetAmount()); // 아이템 버리기
		}
	);

	InventoryUI->OnEquipButtonClicked.BindLambda(
		[this](EItemType _type, uint8 _index)
		{
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
			TWeakObjectPtr<UItem> Item = PlayerManager->GetInventory()->GetItem(_type, _index);

			if (_type >= EItemType::EQUIPABLE)
			{
				TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(Item);
				PlayerManager->GetEquipment()->Equip(Equipment->GetEquipmentType(), Item); // 장착
			}
			else if (_type == EItemType::CONSUMABLE)
			{
				PlayerManager->GetQuickSlot()->Register(InventoryUI->GetOptionalIndex(), Item); // 퀵슬롯 등록
			}
		}
	);

	InventoryUI->OnUnequipButtonClicked.BindLambda(
		[this](EItemType _type, uint8 _index)
		{
			TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
			TWeakObjectPtr<UItem> Item = PlayerManager->GetInventory()->GetItem(_type, _index);

			if (_type >= EItemType::EQUIPABLE)
			{
				TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(Item);
				PlayerManager->GetEquipment()->Unequip(Equipment->GetEquipmentType()); // 장착 해제
			}
			else if (_type == EItemType::CONSUMABLE)
			{
				PlayerManager->GetQuickSlot()->Unregister(InventoryUI->GetOptionalIndex()); // 퀵슬롯 등록 해제
			}
		}
	);

}

void ANonCombatHUD::ToggleMaintenanceUI()
{
	if (MaintenanceUI == nullptr)
		return;

	if (MaintenanceUI->IsShowing())
		MaintenanceUI->HideUI();
	else
		MaintenanceUI->ShowUI();
}

void ANonCombatHUD::ToggleInventoryUI()
{
	if (InventoryUI == nullptr)
		return;

	if (InventoryUI->IsShowing())
		InventoryUI->HideUI();
	else
		InventoryUI->ShowUI();
}

void ANonCombatHUD::ToggleSkillDevelopUI()
{
	if (SkillDevelopUI == nullptr)
		return;

	if (SkillDevelopUI->IsShowing())
		SkillDevelopUI->HideUI();
	else
		SkillDevelopUI->ShowUI();
}
