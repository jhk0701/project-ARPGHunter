// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWPlayerStatusBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "UI/UserWidget/UWPoolingContainer.h"

void UUWStatEffectSlot::SetTexture(UTexture2D* _tex)
{
	Thumbnail->SetBrushFromTexture(_tex);
}

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

void UUWPlayerStatusBar::RegisterStatEffect(UObject* _effectID, UTexture2D* _tex)
{
	if (MapStatEffectSlot.Contains(_effectID) || _tex == nullptr)
		return;

	TWeakObjectPtr<UUserWidget> SlotInst = StatEffectIndicator->GetSlot();
	TObjectPtr<UUWStatEffectSlot> EffectSlot = Cast<UUWStatEffectSlot>(SlotInst);
	EffectSlot->SetTexture(_tex);

	MapStatEffectSlot.Add(_effectID, SlotInst);
}

void UUWPlayerStatusBar::RemoveStatEffect(UObject* _effectID)
{
	if (MapStatEffectSlot.Contains(_effectID) == false)
		return;

	StatEffectIndicator->ReleaseSlot(MapStatEffectSlot[_effectID]);
	MapStatEffectSlot.Remove(_effectID);
}
