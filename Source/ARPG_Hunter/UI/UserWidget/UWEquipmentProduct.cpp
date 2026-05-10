// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UserWidget/UWEquipmentProduct.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager/DataManager.h"
#include "Core/Subsystem/PlayerManager/PlayerManager.h"
#include "Player/Inventory/Inventory.h"
#include "Item/Item.h"
#include "Data/ItemData.h"
#include "Data/ItemProductData.h"

#include "UI/UserWidget/UWStatInfo.h"
#include "UI/UserWidget/UWEquipmentUtilSlot.h"


void UUWEquipmentProduct::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWEquipmentProduct::HideUI);
	ProductButton->OnClicked.AddDynamic(this, &UUWEquipmentProduct::ClickProductButton);
	CompleteButton->OnClicked.AddDynamic(this, &UUWEquipmentProduct::ClickCompleteButton);

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	DataManager->GetAllItemProduct(DataArray);

	if (ProductSlotClass)
	{
		ProductSlotInst.SetNum(DataArray.Num());
		for (uint8 i = 0; i < DataArray.Num(); ++i)
		{
			FItemData* ItemData = DataManager->GetItemData(DataArray[i]->ItemID);
			TObjectPtr<UUWListElementSlot> SlotInst = CreateWidget<UUWListElementSlot>(GetWorld(), ProductSlotClass);

			SlotInst->SetIndex(i);
			SlotInst->SetSlot(ItemData);
			SlotInst->OnSlotClicked.BindUObject(this, &UUWEquipmentProduct::ClickProductSlot);

			ProductSlotInst[i] = SlotInst;
			ProductSlotContainer->AddChild(SlotInst);
		}
	}

	if (StatInfoUIClass)
	{
		for (uint8 i = 0; i < static_cast<uint8>(ECharacterStatType::END); ++i)
		{
			ECharacterStatType Type = static_cast<ECharacterStatType>(i);
			TObjectPtr<UUWStatInfo> Inst = CreateWidget<UUWStatInfo>(GetWorld(), StatInfoUIClass);
			Inst->SetStatName(Type);

			MapStatInfo.Add(Type, Inst);
			StatInfoContainer->AddChild(Inst);
		}
	}

	if (IngredientSlotClass) 
	{
		IngredientSlotInst.SetNum(InitIngredientSlotCount);
		for (uint8 i = 0; i < InitIngredientSlotCount; ++i)
		{
			TObjectPtr<UUWIngredientSlot> Inst = CreateWidget<UUWIngredientSlot>(GetWorld(), IngredientSlotClass);
			IngredientSlotContainer->AddChild(Inst);
			IngredientSlotInst[i] = Inst;
		}
	}
}

void UUWEquipmentProduct::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
	Init();
}

void UUWEquipmentProduct::Init()
{
	CurIndex = 0;
	IngredientDetail->SetVisibility(ESlateVisibility::Hidden);
	ProductResult->SetVisibility(ESlateVisibility::Hidden);
}

void UUWEquipmentProduct::ClickProductSlot(uint8 _index)
{
	CurIndex = _index;
	UpdateDetail();
}

void UUWEquipmentProduct::UpdateDetail()
{
	const FItemProductData* ProductData = DataArray[CurIndex];
	TObjectPtr<UPlayerManager> PlayerManager = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	FItemData* ItemData = DataManager->GetItemData(ProductData->ItemID);
	if (ItemData == nullptr)
		return;

	TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(ItemData->Item);

	// 아이템 정보 UI 갱신
	ItemNameLabel->SetText(FText::FromString(ItemData->Item->Name));
	ItemTypeLabel->SetText(EnumToText(ItemData->Type));
	ItemRankLabel->SetText(FText::Format(FText::FromString(TEXT("Rank {0}")), EquipmentConfig->Rank));

	// 아이템 스펙 정보 기입
	for (const TPair<ECharacterStatType, TObjectPtr<UUWStatInfo>>& Pair : MapStatInfo)
		Pair.Value->SetVisibility(ESlateVisibility::Collapsed);

	for (const TPair<ECharacterStatType, uint32>& Pair : EquipmentConfig->Stat)
	{
		MapStatInfo[Pair.Key]->SetVisibility(ESlateVisibility::Visible);
		MapStatInfo[Pair.Key]->SetStatValue(Pair.Value);
	}

	// 제작 재료 UI 갱신
	// 부족한 경우 보충
	if (ProductData->Ingredients.Num() > IngredientSlotInst.Num())
	{
		TObjectPtr<UUWIngredientSlot> Inst = CreateWidget<UUWIngredientSlot>(GetWorld(), IngredientSlotClass);
		IngredientSlotContainer->AddChild(Inst);
		IngredientSlotInst.Add(Inst);
	}

	bIngredientIsEnough = true;

	TWeakObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
	uint8 i = 0;
	FText IngredientFormat = FText::FromString(TEXT("{0} / {1}"));
	for (; i < ProductData->Ingredients.Num(); ++i)
	{
		const FItemData* IngredientData = DataManager->GetItemData(ProductData->Ingredients[i].ID);
		if (IngredientData == nullptr)
		{
			--i;
			continue;
		}

		uint8 Idx = 0;
		uint16 Amount = 0;
		if (Inventory->TryFindItem(IngredientData->Type, ProductData->Ingredients[i].ID, Idx))
			Amount = Inventory->GetItem(IngredientData->Type, Idx)->GetAmount();
		
		bool bIsEnough = Amount >= ProductData->Ingredients[i].RequireAmount;

		IngredientSlotInst[i]->SetSlot(IngredientData);
		IngredientSlotInst[i]->SetAmountLabel(FText::Format(IngredientFormat, Amount, ProductData->Ingredients[i].RequireAmount), bIsEnough);
		IngredientSlotInst[i]->SetVisibility(ESlateVisibility::Visible);

		bIngredientIsEnough &= bIsEnough;
	}

	for (; i < IngredientSlotInst.Num(); ++i)
		IngredientSlotInst[i]->SetVisibility(ESlateVisibility::Collapsed);

	FText GoldFormat = FText::FromString(TEXT("{0} / {1} G"));
	GoldSlot->SetAmountLabel(FText::Format(GoldFormat, PlayerManager->GetGold(), ProductData->GoldCost), PlayerManager->GetGold() >= ProductData->GoldCost);
	
	bGoldIsEnough = PlayerManager->GetGold() >= ProductData->GoldCost;

	// 제작 버튼 활성화
	ProductButton->SetIsEnabled(bGoldIsEnough && bIngredientIsEnough);

	IngredientDetail->SetVisibility(ESlateVisibility::Visible);
}

void UUWEquipmentProduct::ClickProductButton()
{
	// 버튼을 클릭하기 위해선 재료들을 모두 가지고 있을 것
	if (DataArray.Num() <= CurIndex)
		return;

	FItemProductData* ProductData = DataArray[CurIndex];

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	TObjectPtr<UPlayerManager> Player = GetGameInstance()->GetSubsystem<UPlayerManager>();
	TWeakObjectPtr<UInventory> Inventory = Player->GetInventory();
	
	// 비용 지불
	Player->TrySubGold(ProductData->GoldCost);

	for (const FIngredient& Ingredient : ProductData->Ingredients)
	{
		uint8 Idx = 0;
		FItemData* IngredientData = DataManager->GetItemData(Ingredient.ID);

		Inventory->TryFindItem(IngredientData->Type, Ingredient.ID, Idx);
		Inventory->TrySubItem(IngredientData->Type, Idx, Ingredient.RequireAmount);
	}

	// 아이템 인스턴스 추가
	Player->AddItem(ProductData->ItemID, 1);
	UpdateDetail();

	FItemData* ItemData = DataManager->GetItemData(ProductData->ItemID);
	ProductItemThumbnail->SetBrushFromTexture(ItemData->Item->Thumbnail);
	ProductItemLabel->SetText(FText::FromString(ItemData->Item->Name));
	ProductResult->SetVisibility(ESlateVisibility::Visible);
}

void UUWEquipmentProduct::ClickCompleteButton()
{
	ProductResult->SetVisibility(ESlateVisibility::Hidden);
}
