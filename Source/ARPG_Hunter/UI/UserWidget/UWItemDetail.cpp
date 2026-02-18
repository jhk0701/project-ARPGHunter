// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWItemDetail.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

#include "Define/Enum.h"
#include "Item/Item.h"
#include "Data/ItemData.h"
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
			StatInfoContainer->AddChild(Inst);
		}
	}
}

void UUWItemDetail::SetDetail(TObjectPtr<UItem> _item)
{
}
