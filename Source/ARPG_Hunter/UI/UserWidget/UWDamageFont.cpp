// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWDamageFont.h"
#include "Components/TextBlock.h"


void UUWDamageFont::SetDamage(uint32 _amount, bool _bIsCritical)
{
	DamageLabel->SetText(FText::FromString(FString::FromInt(_amount)));
	DamageLabel->SetColorAndOpacity(_bIsCritical ? ColorOnCritical : ColorOnNormal);
}

void UUWDamageFont::PlayAnim()
{
	if (MoveAnim)
		PlayAnimation(MoveAnim);
}
