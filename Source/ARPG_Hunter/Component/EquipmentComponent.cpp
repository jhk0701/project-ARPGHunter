// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipmentComponent.h"
#include "Data/WeaponTypeProfile.h"

UEquipmentComponent::UEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// TODO : 플레이어 장착 정보 기반으로 WeaponProfile 설정
	static ConstructorHelpers::FObjectFinder<UWeaponTypeProfile> TmpProfileFinder(TEXT("/Script/ARPG_Hunter.WeaponTypeProfile'/Game/03-Data/WeaponTypeProfile/SwordProfile.SwordProfile'"));
	if (TmpProfileFinder.Succeeded())
		WeaponProfile = TmpProfileFinder.Object;
}

void UEquipmentComponent::Init()
{
}

UAnimMontage* UEquipmentComponent::GetDodgeMontage()
{
	return WeaponProfile->DodgeMontage;
}
