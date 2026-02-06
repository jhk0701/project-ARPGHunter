// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManager.generated.h"

class UDataTable;
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
	TObjectPtr<UDataTable> WeaponTypeDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> MonsterDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> RegionDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> StageDataTable;

public:
	UDataManager();
	
	struct FWeaponTypeData* GetWeaponTypeData(EWeaponType _type) const;
	struct FMonsterData* GetMonsterData(const FName& _id) const;
	struct FRegionData* GetRegionData(const FName& _id) const;
	struct FStageData* GetStageData(const FName& _id) const;
};
