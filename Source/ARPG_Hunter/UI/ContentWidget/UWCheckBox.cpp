// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ContentWidget/UWCheckBox.h"

void UUWCheckBox::Init()
{
	OnCheckStateChanged.AddDynamic(this, &UUWCheckBox::ClickCheckBox);
}

void UUWCheckBox::ClickCheckBox(bool _bIsChecked)
{
	OnCheckBoxChanged.Broadcast(_bIsChecked, Option);
}

void UUWCheckBox::UpdateStateWithoutEvent(bool _bIsChecked)
{
	FOnCheckBoxComponentStateChanged Tmp = OnCheckStateChanged;
	OnCheckStateChanged.Clear();

	SetIsChecked(_bIsChecked);

	OnCheckStateChanged = Tmp;
}
