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

void UUWInputGuide::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	Super::ShowUI(_bIsSubUI, _mainUI);
	ClickPageCategory(0);
}

void UUWInputGuide::ClickPageCategory(uint8 _idx)
{
	Pages->SetActiveWidgetIndex(_idx);
	PageCategory->SetSelectElementManually(_idx);
}

void UUWInputGuide::ClickPrev()
{
	PageCategory->SetPrevElement();
	Pages->SetActiveWidgetIndex(PageCategory->GetSelectedOption());
}

void UUWInputGuide::ClickNext()
{
	PageCategory->SetNextElement();
	Pages->SetActiveWidgetIndex(PageCategory->GetSelectedOption());
}