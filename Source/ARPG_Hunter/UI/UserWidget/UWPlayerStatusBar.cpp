// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWPlayerStatusBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUWPlayerStatusBar::SetHealthBarPercent(uint16 _remain, uint16 _max)
{
	HealthBar->SetPercent(static_cast<float>(_remain) / _max);
	HealthLabel->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), _remain, _max)));
}

void UUWPlayerStatusBar::SetStaminaBarPercent(uint16 _remain, uint16 _max)
{
	StaminaBar->SetPercent(static_cast<float>(_remain) / _max);
	StaminaLabel->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), _remain, _max)));
}

void UUWPlayerStatusBar::SetSkillBarPercent(uint16 _remain, uint16 _max)
{
	SkillBar->SetPercent(static_cast<float>(_remain) / _max);
}
