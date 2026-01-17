// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipmentComponent.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
}
