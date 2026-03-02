// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UserWidget/UWEquipmentUpgrade.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include "Define/Enum.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Core/Subsystem/DataManager.h"
#include "Player/Inventory.h"
#include "Item/Item.h"
#include "Data/ItemData.h"
#include "Data/EquipmentUpgradeData.h"

#include "UI/UserWidget/UWCategory.h"
#include "UI/UserWidget/UWEquipmentUtilSlot.h"
#include "UI/UserWidget/UWStatInfo.h"


void UUWEquipmentUpgrade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Inventory = GetGameInstance()->GetSubsystem<UPlayerManager>()->GetInventory();

	CloseButton->OnClicked.AddDynamic(this, &UUWEquipmentUpgrade::HideUI);
	ItemCategory->OnSelected.AddUObject(this, &UUWEquipmentUpgrade::SelectCategory);
	
	if (EquipmentSlotClass) 
	{
		uint8 Size = Inventory->GetContainerSize();
		
		EquipmentSlotInst.SetNum(Size);
		for (uint8 i = 0; i < Size; ++i)
		{
			TObjectPtr<UUWListElementSlot> SlotInst = CreateWidget<UUWListElementSlot>(GetWorld(), EquipmentSlotClass);
			SlotInst->SetIndex(i);
			SlotInst->OnSlotClicked.BindUObject(this, &UUWEquipmentUpgrade::SelectSlot);
			SlotInst->SetVisibility(ESlateVisibility::Collapsed);

			EquipmentSlotInst[i] = SlotInst;
			EquipmentSlotContainer->AddChild(SlotInst);
		}
	}

	if (IngredientSlotClass) 
	{
		IngredientSlotInst.SetNum(InitIngredientSlotSize);
		for (uint8 i = 0; i < InitIngredientSlotSize; ++i)
		{
			TObjectPtr<UUWIngredientSlot> SlotInst = CreateWidget<UUWIngredientSlot>(GetWorld(), IngredientSlotClass);
			IngredientSlotInst[i] = SlotInst;
			IngredientInfo->AddChild(SlotInst);
			SlotInst->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (StatChangeClass)
	{
		for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
		{
			ECharacterStatType Type = static_cast<ECharacterStatType>(i);

			TObjectPtr<UUWStatChangeCompare> ChangeInst = CreateWidget<UUWStatChangeCompare>(GetWorld(), StatChangeClass);
			ChangeInst->SetStatName(Type);
			ChangeInst->SetVisibility(ESlateVisibility::Collapsed);
			StatChangeInst.Add(Type, ChangeInst);
			UpgradeStatContainer->AddChild(ChangeInst);
		}
	}
}

void UUWEquipmentUpgrade::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);
	Init();
}

void UUWEquipmentUpgrade::Init()
{
	// 플레이어 장비 출력
	SelectCategory(static_cast<uint32>(EItemType::WEAPON));

	Result->SetVisibility(ESlateVisibility::Hidden);
}

void UUWEquipmentUpgrade::SelectCategory(uint8 _option)
{
	if (Inventory.IsValid() == false)
		return;

	CurItemType = static_cast<EItemType>(_option);

	const TArray<TObjectPtr<UItem>>& Items = Inventory->GetContainer(CurItemType);

	for (uint8 i = 0; i < Items.Num(); ++i)
	{
		if (Items[i])
		{
			EquipmentSlotInst[i]->SetSlot(Items[i]);
			EquipmentSlotInst[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
			EquipmentSlotInst[i]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUWEquipmentUpgrade::SelectSlot(uint8 _index)
{
	if (Inventory.IsValid() == false)
		return;

	TWeakObjectPtr<UItem> Items = Inventory->GetContainer(CurItemType)[_index];
	if (Items.IsValid() == false)
		return;

	TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(Items);
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();

	TObjectPtr<UEquipmentItemConfig> Config = Cast<UEquipmentItemConfig>(Equipment->GetConfig());
	ItemThumbnail->SetBrushFromTexture(Config->Thumbnail);
	ItemNameLabel->SetText(FText::FromString(Config->Name));

	FEquipmentUpgradeData* UpgradeData = DataManager->GetUpgradeData(Config->Rank, Equipment->GetGrade(), Config->Type);
	FEquipmentUpgradeData* NextUpgradeData = DataManager->GetUpgradeData(Config->Rank, Equipment->GetGrade() + 1, Config->Type);

	if (UpgradeData == nullptr || NextUpgradeData == nullptr)
		return;

	FText GradeFormat = FText::FromString(TEXT("+{0}"));
	CurGradeLabel->SetText(FText::Format(GradeFormat, Equipment->GetGrade()));
	NextGradeLabel->SetText(FText::Format(GradeFormat, Equipment->GetGrade() + 1));

	// 성공률 출력
	SucceessPercentLabel->SetText(FText::Format(FText::FromString(TEXT("{0} %")), UpgradeData->SuccessPercent));

	// 강화 수치
	// 기본 스탯 + 강화 스탯
	TMap<ECharacterStatType, uint16> CurStat;
	Equipment->GetUpgradeStat(GetWorld(), CurStat);

	// 다음 강화 스탯 결과
	FText StatChangeFormat = FText::FromString(TEXT("{0} (+{1})"));
	for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
	{
		ECharacterStatType Type = static_cast<ECharacterStatType>(i);
		uint16* UpgradeStat = NextUpgradeData->StatPerStep.Find(Type);
		if (UpgradeStat == nullptr)
		{
			StatChangeInst[Type]->SetVisibility(ESlateVisibility::Collapsed);
			continue;
		}

		StatChangeInst[Type]->SetVisibility(ESlateVisibility::Visible);

		uint16 Stat = CurStat.Find(Type) == nullptr ? 0 : CurStat[Type];
		StatChangeInst[Type]->SetBeforeStatValue(Stat);
		StatChangeInst[Type]->SetStatValue(FText::Format(StatChangeFormat, (Stat + *UpgradeStat), *UpgradeStat));
	}
	
	// 재료 출력
	bool bIngredientIsEnough = true;
	FText IngredientFormat = FText::FromString(TEXT("{0} / {1}"));
	uint8 Idx = 0;
	for (const FUpgradeIngredient& Ingredient : UpgradeData->Ingredients)
	{
		FItemData* IngredientData = DataManager->GetItemData(Ingredient.ItemID);

		uint8 InvenIdx = 0;
		uint32 OwnAmount = 0;
		if (Inventory->TryFindItem(IngredientData->Type, Ingredient.ItemID, InvenIdx)) 
			OwnAmount = Inventory->GetItem(IngredientData->Type, InvenIdx)->GetAmount();

		bIngredientIsEnough &= (OwnAmount >= Ingredient.Amount);

		TArray<FText> Addictive = { FText::Format(IngredientFormat, OwnAmount, Ingredient.Amount) };
		IngredientSlotInst[Idx]->SetSlot(IngredientData, &Addictive);
		IngredientSlotInst[Idx]->SetVisibility(ESlateVisibility::Visible);
		Idx++;
	}

	for (; Idx < IngredientSlotInst.Num(); ++Idx)
		IngredientSlotInst[Idx]->SetVisibility(ESlateVisibility::Collapsed);

	// 골드 반영
	bool bGoldIsEnough = PlayerManager->GetGold() >= UpgradeData->GoldCost;

	FText GoldFormat = FText::FromString(TEXT("{0} / {1} G"));
	GoldSlot->SetAmountLabel(FText::Format(GoldFormat, PlayerManager->GetGold(), UpgradeData->GoldCost), bGoldIsEnough);

	UpgradeButton->SetIsEnabled(bIngredientIsEnough && bGoldIsEnough);
}