// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemDetail.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/WrapBox.h"

#include "Define/Enum.h"
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
	TObjectPtr<UItemConfig> Config = _item->GetConfig();
	NameLabel->SetText(FText::FromString(Config->Name));
	DescLabel->SetText(FText::FromString(Config->Desc));

	ConsumableInfo->SetVisibility(ESlateVisibility::Collapsed);
	EquipmentInfo->SetVisibility(ESlateVisibility::Collapsed);

	if (_item->GetType() == EItemType::CONSUMABLE)
	{
		TypeLabel->SetText(FText::FromString(TEXT("소비품")));

		TObjectPtr<UConsumableItemConfig> ConsumableConfig = Cast<UConsumableItemConfig>(Config);
		AmountLabel->SetText(FText::AsNumber(ConsumableConfig->AllowCountOnCombat));
		CooldownLabel->SetText(FText::AsTime(ConsumableConfig->Cooldown));

		ConsumableInfo->SetVisibility(ESlateVisibility::Visible);
	}
	else if (_item->GetType() >= EItemType::EQUIPABLE)
	{
		if (_item->GetType() == EItemType::WEAPON) 
			TypeLabel->SetText(FText::FromString(TEXT("무기")));
		else 
			TypeLabel->SetText(FText::FromString(TEXT("방어구")));

		for (const TPair<ECharacterStatType, TObjectPtr<UUWStatInfo>>& Pair : MapStatInfo)
			Pair.Value->SetVisibility(ESlateVisibility::Collapsed);

		TObjectPtr<UEquipmentItemConfig> EquipmentConfig = Cast<UEquipmentItemConfig>(Config);
		for (const TPair<ECharacterStatType, uint32>& Pair : EquipmentConfig->Stat)
		{
			MapStatInfo[Pair.Key]->SetVisibility(ESlateVisibility::Visible);
			MapStatInfo[Pair.Key]->SetStatValue(Pair.Value);
		}

		EquipmentInfo->SetVisibility(ESlateVisibility::Visible);
	}
	else
		TypeLabel->SetText(FText::FromString(TEXT("일반 재료")));
}