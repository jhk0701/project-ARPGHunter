// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipmentComponent.h"
#include "Define/Define.h"
#include "Subsystem/DataManager/DataManager.h"
#include "Data/WeaponTypeData.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UEquipmentComponent::Init()
{
	UDataManager* DataManager = GetOwner()->GetGameInstance()->GetSubsystem<UDataManager>();
	CurWeaponType = DataManager->GetWeaponTypeData(EWeaponType::SWORD);
}

UAnimMontage* UEquipmentComponent::GetDodgeMontage()
{
	return  CurWeaponType->DodgeMontage;
}
