// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UserWidget/UWEquipmentProduct.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Border.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager.h"
#include "Core/Subsystem/PlayerManager.h"
#include "Player/Inventory.h"
#include "Item/Item.h"
#include "Data/ItemData.h"
#include "Data/ItemProductData.h"
#include "UI/UserWidget/UWStatInfo.h"

#pragma region Sub Slot

void UUWProductSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SlotButton->OnClicked.AddDynamic(this, &UUWProductSlot::ClickSlot);
}

void UUWProductSlot::SetSlot(const FText& _nameText, TObjectPtr<UTexture2D> _thumbnail)
{
	NameLabel->SetText(_nameText);
	Thumbnail->SetBrushFromTexture(_thumbnail);
}

void UUWProductSlot::ClickSlot()
{
	OnProductSlotClicked.ExecuteIfBound(Index);
}

void UUWIngredientSlot::SetSlot(const FText& _nameText, const FText& _amountText, TObjectPtr<UTexture2D> _thumbnail)
{
	NameLabel->SetText(_nameText);
	AmountLabel->SetText(_amountText);
	Thumbnail->SetBrushFromTexture(_thumbnail);
}

#pragma endregion

void UUWEquipmentProduct::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWEquipmentProduct::HideUI);
	ProductButton->OnClicked.AddDynamic(this, &UUWEquipmentProduct::ClickProductButton);

	TObjectPtr<UDataManager> DataManager = GetGameInstance()->GetSubsystem<UDataManager>();
	DataManager->GetAllItemProduct(DataArray);

	if (ProductSlotClass)
	{
		ProductSlotInst.SetNum(DataArray.Num());
		for (uint8 i = 0; i < DataArray.Num(); ++i)
		{
			FItemData* ItemData = DataManager->GetItemData(DataArray[i]->ItemID);
			TObjectPtr<UUWProductSlot> SlotInst = CreateWidget<UUWProductSlot>(GetWorld(), ProductSlotClass);

			SlotInst->SetIndex(i);
			SlotInst->SetSlot(FText::FromString(ItemData->Item->Name), ItemData->Item->Thumbnail);
			SlotInst->OnProductSlotClicked.BindUObject(this, &UUWEquipmentProduct::ClickProductSlot);

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

void UUWEquipmentProduct::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);
	Init();
}

void UUWEquipmentProduct::Init()
{
	CurIndex = 0;
	IngredientDetail->SetVisibility(ESlateVisibility::Hidden);
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

	// 아이템 정보 UI 갱신
	ItemNameLabel->SetText(FText::FromString(ItemData->Item->Name));
	ItemTypeLabel->SetText(EnumToText(ItemData->Type));

	// 아이템 스펙 정보 기입
	for (const TPair<ECharacterStatType, TObjectPtr<UUWStatInfo>>& Pair : MapStatInfo)
		Pair.Value->SetVisibility(ESlateVisibility::Collapsed);

	TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(ItemData->Item);
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

	TObjectPtr<UInventory> Inventory = PlayerManager->GetInventory();
	uint8 i = 0;
	for (; i < ProductData->Ingredients.Num(); ++i)
	{
		const FItemData* IngredientData = DataManager->GetItemData(ProductData->Ingredients[i].ID);
		if (IngredientData == nullptr)
		{
			--i;
			continue;
		}

		uint8 Idx = 0;
		uint8 Amount = 0;
		if(Inventory->TryFindItem(IngredientData->Type, ProductData->Ingredients[i].ID, Idx))
			Amount = Inventory->GetItem(IngredientData->Type, Idx)->GetAmount();

		FString StrAmount = FString::Printf(TEXT("%d / %d"), Amount, ProductData->Ingredients[i].RequireAmount);
		IngredientSlotInst[i]->SetSlot(
			FText::FromString(IngredientData->Item->Name), 
			FText::FromString(StrAmount),
			IngredientData->Item->Thumbnail
		);

		IngredientSlotInst[i]->SetVisibility(ESlateVisibility::Visible);

		bIngredientIsEnough = bIngredientIsEnough && Amount >= ProductData->Ingredients[i].RequireAmount;
	}

	for (; i < IngredientSlotInst.Num(); ++i)
		IngredientSlotInst[i]->SetVisibility(ESlateVisibility::Collapsed);

	FString StrGold = FString::Printf(TEXT("%d / %d"), PlayerManager->GetGold(), ProductData->GoldCost);
	GoldLabel->SetText(FText::FromString(StrGold));
	
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
	TObjectPtr<UInventory> Inventory = Player->GetInventory();
	
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
}
