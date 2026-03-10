// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInputGuide.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

#include "UI/UserWidget/UWCategory.h"

void UUWInputGuide::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWInputGuide::HideUI);

	PageCategory->OnSelected.AddUObject(this, &UUWInputGuide::ClickPageCategory);
	PrevButton->OnClicked.AddDynamic(this, &UUWInputGuide::ClickPrev);
	NextButton->OnClicked.AddDynamic(this, &UUWInputGuide::ClickNext);
}

void UUWInputGuide::ShowUI(bool _bIsSubUI)
{
	Super::ShowUI(_bIsSubUI);
	ClickPageCategory(0);
}

void UUWInputGuide::ClickPageCategory(uint8 _idx)
{
	Index = _idx;
	Pages->SetActiveWidgetIndex(_idx);
}

void UUWInputGuide::ClickPrev()
{
	if (Index == 0)
		return;

	ClickPageCategory(--Index);
	PageCategory->SetElementSelected(Index);
}

void UUWInputGuide::ClickNext()
{
	if (Index == Pages->GetNumWidgets() - 1)
		return;

	ClickPageCategory(++Index);
	PageCategory->SetElementSelected(Index);
}
