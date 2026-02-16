// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWMaintenanceWindow.h"

#include "Components/Button.h"

void UUWMaintenanceWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWMaintenanceWindow::ClickCloseButton);
}

void UUWMaintenanceWindow::ClickCloseButton()
{
	HideUI();
}
