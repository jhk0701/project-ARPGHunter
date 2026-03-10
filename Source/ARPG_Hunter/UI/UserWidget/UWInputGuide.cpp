// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWInputGuide.h"
#include "Components/Button.h"

void UUWInputGuide::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWInputGuide::HideUI);
}
