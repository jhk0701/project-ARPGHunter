// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWMonsterStatusBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UUWMonsterStatusBar::SetHealthBarPercent(uint16 _remain, uint16 _max)
{
	HealthBar->SetPercent(static_cast<float>(_remain) / _max);
	HealthLabel->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), _remain, _max)));
}

void UUWBossMonsterStatusBar::SetStaggerBarPercent(uint16 _remain, uint16 _max)
{
	StaggerBar->SetPercent(static_cast<float>(_remain) / _max);
}

void UUWBossMonsterStatusBar::SetGimicStaggerBarPercent(uint16 _remain, uint16 _max)
{
	GimicStaggerBar->SetPercent(static_cast<float>(_remain) / _max);
}

void UUWBossMonsterStatusBar::ShowUI()
{
	HideGimicStagger();
	AddToViewport();
}

void UUWBossMonsterStatusBar::HideUI()
{
	RemoveFromViewport();
}

void UUWBossMonsterStatusBar::ShowGimicStagger()
{
	GimicStaggerBar->SetVisibility(ESlateVisibility::Visible);
}

void UUWBossMonsterStatusBar::HideGimicStagger()
{
	GimicStaggerBar->SetVisibility(ESlateVisibility::Hidden);
}
