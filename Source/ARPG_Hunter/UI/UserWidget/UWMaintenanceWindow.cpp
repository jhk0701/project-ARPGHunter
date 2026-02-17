// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWMaintenanceWindow.h"
#include "Components/Button.h"
#include "Components/NamedSlot.h"

void UUWMaintenanceWindow::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UUWMaintenanceWindow::ClickCloseButton);
}

void UUWMaintenanceWindow::SetInventoryUI(TObjectPtr<UUserWidget> _ui)
{
	InventorySlot->SetContent(_ui);
}

void UUWMaintenanceWindow::SetEquipmentUI(TObjectPtr<UUserWidget> _ui)
{
	EquipmentSlot->SetContent(_ui);
}

void UUWMaintenanceWindow::ClickCloseButton()
{
	HideUI();

	InventorySlot->SetContent(nullptr);
	EquipmentSlot->SetContent(nullptr);
}
