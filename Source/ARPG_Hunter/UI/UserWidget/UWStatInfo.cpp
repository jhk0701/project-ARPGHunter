// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWStatInfo.h"
#include "Components/TextBlock.h"

#include "Define/Enum.h"

void UUWStatInfo::SetStatName(ECharacterStatType _type)
{
	NameLabel->SetText(EnumToText(_type));
}

void UUWStatInfo::SetStatValue(uint32 _value)
{
	ValueLabel->SetText(FText::AsNumber(_value));
}

void UUWStatInfo::SetStatValue(const FText& _text)
{
	ValueLabel->SetText(_text);
}

void UUWStatChangeCompare::SetBeforeStatValue(uint32 _value)
{
	BeforeValueLabel->SetText(FText::AsNumber(_value));
}
