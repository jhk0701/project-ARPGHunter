// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipmentComponent.h"
#include "Data/WeaponTypeData.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UEquipmentComponent::Init()
{
}

UAnimMontage* UEquipmentComponent::GetDodgeMontage()
{
	return  nullptr;//;WeaponProfile->DodgeMontage;
}
