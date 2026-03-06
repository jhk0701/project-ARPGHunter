// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ARPGGameInstance.h"
#include "Core/Subsystem/PlayerManager.h"

void UARPGGameInstance::OnStart()
{
	Super::OnStart();

	GetSubsystem<UPlayerManager>()->PostInit();
}
