// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWStatInfo.h"
#include "Components/TextBlock.h"

#include "Define/Enum.h"

void UUWStatInfo::SetStatName(ECharacterStatType _type)
{
	NameLabel->SetText(FText::FromName(EnumToName(_type)));
}

void UUWStatInfo::SetStatValue(uint32 _value)
{
	ValueLabel->SetText(FText::FromString(FString::FromInt(_value)));
}

void UUWStatInfo::SetStatValue(uint32 _baseValue, uint32 _equipmentValue)
{
	FString str = FString::Printf(TEXT("%d + (%d) = %d"), _baseValue, _equipmentValue, (_baseValue + _equipmentValue));
	ValueLabel->SetText(FText::FromString(str));
}
