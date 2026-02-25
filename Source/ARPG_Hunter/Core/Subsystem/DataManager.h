// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManager.generated.h"

class UDataTable;
class UWeaponType;
class UWeaponConfig;
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
	TObjectPtr<UWeaponType> WeaponTypeData;
	UPROPERTY()
	TObjectPtr<UDataTable> MonsterDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> RegionDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> StageDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> ItemDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> ItemProductDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> DialogDataTable;

public:
	UDataManager();
	
	TObjectPtr<UWeaponConfig> GetWeaponConfig(EWeaponType _type) const;
	struct FMonsterData* GetMonsterData(const FName& _id) const;
	struct FRegionData* GetRegionData(const FName& _id) const;
	struct FStageData* GetStageData(const FName& _id) const;
	struct FItemData* GetItemData(const FName& _id) const;
	struct FItemProductData* GetItemProductData(const FName& _id) const;
	struct FDialogData* GetDialogData(const FName& _id) const;
};
