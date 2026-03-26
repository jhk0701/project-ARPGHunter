// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWEquipmentSuccession.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/Border.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Core/Subsystem/DataManager.h"
#include "Player/Inventory.h"
#include "Player/Equipment.h"
#include "Data/ItemData.h"
#include "Data/EquipmentSuccessData.h"
#include "Item/Item.h"

#include "UI/UserWidget/UWCategory.h"
#include "UI/UserWidget/UWEquipmentUtilSlot.h"

void UUWEquipmentSuccession::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ItemCategory->OnSelected.AddUObject(this, &UUWEquipmentSuccession::SelectCategory);
	CloseButton->OnClicked.AddDynamic(this, &UUWEquipmentSuccession::HideUI);
	SuccessButton->OnClicked.AddDynamic(this, &UUWEquipmentSuccession::SucceessItem);
	ConfirmButton->OnClicked.AddDynamic(this, &UUWEquipmentSuccession::ConfirmResult);

	if (EquipmentSlotClass) 
	{
		TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
		uint8 Size = PlayerManager->GetInventory()->GetContainerSize();

		EquipmentSlotInst.SetNum(Size);
		for (uint8 i = 0; i < Size; ++i)
		{
			TObjectPtr<UUWListElementSlot> SlotInst = CreateWidget<UUWListElementSlot>(GetWorld(), EquipmentSlotClass);
			SlotInst->SetIndex(i);
			SlotInst->SetVisibility(ESlateVisibility::Collapsed);
			SlotInst->OnSlotClicked.BindUObject(this, &UUWEquipmentSuccession::ClickEquipmentSlot);
			EquipmentSlotInst[i] = SlotInst;
			EquipmentSlotContainer->AddChild(SlotInst);
		}

		IngredientSlotInst.SetNum(Size);
		for (uint8 i = 0; i < Size; ++i)
		{
			TObjectPtr<UUWListElementSlot> SlotInst = CreateWidget<UUWListElementSlot>(GetWorld(), EquipmentSlotClass);
			SlotInst->SetIndex(i);
			SlotInst->SetVisibility(ESlateVisibility::Collapsed);
			SlotInst->OnSlotClicked.BindUObject(this, &UUWEquipmentSuccession::ClickIngredientSlot);
			IngredientSlotInst[i] = SlotInst;
			IngredientSlotContainer->AddChild(SlotInst);
		}
	}
}

void UUWEquipmentSuccession::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
	Init();
}

void UUWEquipmentSuccession::Init()
{
	SuccessedSlot->Clear();
	IngredientSlot->Clear();
	ResultSlot->Clear();
	Result->SetVisibility(ESlateVisibility::Hidden);

	bSuccessionIsEnable = false;
	GoldSlot->SetAmountLabel(FText::FromString(TEXT("- G")), true);
	SuccessButton->SetIsEnabled(bSuccessionIsEnable);

	for (TObjectPtr<UUWListElementSlot>& ListSlot : IngredientSlotInst)
		ListSlot->SetVisibility(ESlateVisibility::Collapsed);

	EquipmentIdx = 0;
	IngredientIdx = 0;
	SelectCategory(static_cast<uint8>(EItemType::WEAPON));
}

void UUWEquipmentSuccession::SelectCategory(uint8 _opt)
{
	CurItemType = static_cast<EItemType>(_opt);

	TWeakObjectPtr<UInventory> Inventory = GetGameInstance()->GetSubsystem<UPlayerManager>()->GetInventory();

	TSet<uint8> IdxSet;
	Inventory->SearchItems(CurItemType, IdxSet, 
		[](TObjectPtr<UItem> _item) 
		{
			TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(_item);
			return Equipment->GetGrade() == 0;
		}
	);

	for (uint8 i = 0; i < EquipmentSlotInst.Num(); ++i)
	{
		if(IdxSet.Contains(i) == false)
		{
			EquipmentSlotInst[i]->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}
		
		EquipmentSlotInst[i]->SetVisibility(ESlateVisibility::Visible);
		EquipmentSlotInst[i]->SetSlot(Inventory->GetItem(CurItemType, i));
	}
}

void UUWEquipmentSuccession::ClickEquipmentSlot(uint8 _index)
{
	EquipmentIdx = _index;

	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TWeakObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();

	TObjectPtr<UEquipmentSuccessData> SuccessionData = GetGameInstance()->GetSubsystem<UDataManager>()->GetSuccessData();
	uint8 Requirement = SuccessionData->GetRequiredGradeForSuccession();

	TObjectPtr<UEquipmentItem> SelectedItem = Cast<UEquipmentItem>(Inventory->GetItem(CurItemType, EquipmentIdx));
	TObjectPtr<UEquipmentItemConfig> SelectedConfig = Cast<UEquipmentItemConfig>(SelectedItem->GetConfig());

	SuccessedSlot->SetSlot(SelectedItem);

	TSet<uint8> IdxSet;
	Inventory->SearchItems(CurItemType, IdxSet, 
		[SelectedConfig, Requirement](TObjectPtr<UItem> _item)
		{
			TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(_item);
			TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(_item->GetConfig());
			return SelectedConfig->Rank - 1 == EquipmentConfig->Rank && 
				SelectedConfig->Type == EquipmentConfig->Type &&
				Equipment->GetGrade() >= Requirement;
		});

	// 재료 아이템 출력
	for (uint8 i = 0; i < IngredientSlotInst.Num(); ++i)
	{
		if (IdxSet.Contains(i) == false)
		{
			IngredientSlotInst[i]->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}

		IngredientSlotInst[i]->SetVisibility(ESlateVisibility::Visible);
		IngredientSlotInst[i]->SetSlot(Inventory->GetItem(CurItemType, i));
	}

	bSuccessionIsEnable = PlayerManager->GetGold() >= SuccessionData->GoldCost;

	FText GoldFormat = FText::FromString(TEXT("{0} / {1} G"));
	GoldSlot->SetAmountLabel(FText::Format(GoldFormat, PlayerManager->GetGold(), SuccessionData->GoldCost), bSuccessionIsEnable);

	IngredientSlot->Clear();
}

void UUWEquipmentSuccession::ClickIngredientSlot(uint8 _index)
{
	IngredientIdx = _index;

	TWeakObjectPtr<UInventory> Inventory = GetGameInstance()->GetSubsystem<UPlayerManager>()->GetInventory();
	IngredientSlot->SetSlot(Inventory->GetItem(CurItemType, IngredientIdx));

	SuccessButton->SetIsEnabled(bSuccessionIsEnable);
}

void UUWEquipmentSuccession::ConfirmResult()
{
	Result->SetVisibility(ESlateVisibility::Hidden);
}

void UUWEquipmentSuccession::SucceessItem()
{
	TObjectPtr<UEquipmentSuccessData> SuccessionData = GetGameInstance()->GetSubsystem<UDataManager>()->GetSuccessData();

	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	if (PlayerManager->TrySubGold(SuccessionData->GoldCost) == false)
		return;

	TWeakObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
	TWeakObjectPtr<UEquipment> Equipment = PlayerManager->GetEquipment();

	TObjectPtr<UEquipmentItem> Target = Cast<UEquipmentItem>(Inventory->GetItem(CurItemType, EquipmentIdx));
	TObjectPtr<UEquipmentItem> Ingredient = Cast<UEquipmentItem>(Inventory->GetItem(CurItemType, IngredientIdx));
	
	// 장착 중이라면 먼저 일시 해제
	TObjectPtr<UEquipmentItem> CurEquipped = nullptr;
	if (Target->IsEquiped())
	{
		Equipment->Unequip(Target->GetEquipmentType());
		CurEquipped = Target;
	}
	else if (Ingredient->IsEquiped()) 
	{
		Equipment->Unequip(Target->GetEquipmentType());
		CurEquipped = Ingredient;
	}

	// 계승 처리
	uint8 Grade = Ingredient->GetGrade();
	Ingredient->SetGrade(0);
	Target->SetGrade(Grade - SuccessionData->SubtractOnSuccession);

	// 장착 중인 아이템 다시 장착
	if (CurEquipped)
		Equipment->Equip(Target->GetEquipmentType(), CurEquipped);

	ResultSlot->SetSlot(Target);
	Result->SetVisibility(ESlateVisibility::Visible);

	ClickEquipmentSlot(EquipmentIdx);
}
