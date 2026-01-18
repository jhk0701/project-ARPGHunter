// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManager.generated.h"

enum class EWeaponType :uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TObjectPtr<class UDataTable> WeaponTypeDataTable;

public:
	UDataManager();

	struct FWeaponTypeData* GetWeaponTypeData(EWeaponType _type);
};
