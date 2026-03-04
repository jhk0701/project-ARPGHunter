// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemDetail.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"

#include "Define/Enum.h"
#include "Core/Subsystem/DataManager.h"
#include "Data/ItemData.h"
#include "Item/Item.h"
#include "UI/UserWidget/UWStatInfo.h"


void UUWItemDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
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
}

void UUWItemDetail::SetDetail(TWeakObjectPtr<UItem> _item)
{
	FString ItemName;
	_item->GetItemName(ItemName);

	TObjectPtr<UItemConfig> Config = _item->GetConfig();
	NameLabel->SetText(FText::FromString(ItemName));
	DescLabel->SetText(FText::FromString(Config->Desc));

	ConsumableInfo->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentInfo->SetVisibility(ESlateVisibility::Collapsed);

	TypeLabel->SetText(EnumToText(_item->GetType()));
	RankLabel->SetVisibility(_item->GetType() >= EItemType::EQUIPABLE ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (_item->GetType() == EItemType::CONSUMABLE)
	{
		TObjectPtr<UConsumableItemConfig> ConsumableConfig = Cast<UConsumableItemConfig>(Config);
		AmountLabel->SetText(FText::AsNumber(ConsumableConfig->AllowCountOnCombat));
		ConsumableInfo->SetVisibility(ESlateVisibility::Visible);
	}
	else if (_item->GetType() >= EItemType::EQUIPABLE)
	{
		for (const TPair<ECharacterStatType, TObjectPtr<UUWStatInfo>>& Pair : MapStatInfo)
			Pair.Value->SetVisibility(ESlateVisibility::Collapsed);

		TObjectPtr<UEquipmentItem> Equipment = Cast<UEquipmentItem>(_item);
		TMap<ECharacterStatType, uint32> Stat;
		Equipment->GetStat(GetGameInstance()->GetSubsystem<UDataManager>(), Stat);

		for (const TPair<ECharacterStatType, uint32>& Pair : Stat)
		{
			MapStatInfo[Pair.Key]->SetVisibility(ESlateVisibility::Visible);
			MapStatInfo[Pair.Key]->SetStatValue(Pair.Value);
		}
		
		EquipmentInfo->SetVisibility(ESlateVisibility::Visible);
		
		TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(Equipment->GetConfig());
		RankLabel->SetText(FText::Format(FText::FromString(TEXT("Rank {0}")), EquipmentConfig->Rank));
	}

}

void UUWItemDetail::SetDetail(FItemData* _data)
{
	FString ItemName = _data->Item->Name;

	TObjectPtr<UItemConfig> Config = _data->Item;
	NameLabel->SetText(FText::FromString(ItemName));
	DescLabel->SetText(FText::FromString(Config->Desc));

	ConsumableInfo->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentInfo->SetVisibility(ESlateVisibility::Collapsed);

	EItemType Type = _data->Type;
	TypeLabel->SetText(EnumToText(Type));
	RankLabel->SetVisibility(Type >= EItemType::EQUIPABLE ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	if (Type == EItemType::CONSUMABLE)
	{
		TObjectPtr<UConsumableItemConfig> ConsumableConfig = Cast<UConsumableItemConfig>(Config);
		AmountLabel->SetText(FText::AsNumber(ConsumableConfig->AllowCountOnCombat));
		ConsumableInfo->SetVisibility(ESlateVisibility::Visible);
	}
	else if (Type >= EItemType::EQUIPABLE)
	{
		for (const TPair<ECharacterStatType, TObjectPtr<UUWStatInfo>>& Pair : MapStatInfo)
			Pair.Value->SetVisibility(ESlateVisibility::Collapsed);

		TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(Config);
		TMap<ECharacterStatType, uint32> Stat = EquipmentConfig->Stat;

		for (const TPair<ECharacterStatType, uint32>& Pair : Stat)
		{
			MapStatInfo[Pair.Key]->SetVisibility(ESlateVisibility::Visible);
			MapStatInfo[Pair.Key]->SetStatValue(Pair.Value);
		}

		EquipmentInfo->SetVisibility(ESlateVisibility::Visible);
		RankLabel->SetText(FText::Format(FText::FromString(TEXT("Rank {0}")), EquipmentConfig->Rank));
	}
}
