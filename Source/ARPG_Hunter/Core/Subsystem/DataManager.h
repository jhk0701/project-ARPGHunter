// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DataManager.generated.h"

class UDataTable;
class UWeaponType;
class UWeaponConfig;
class UItemTradeConfig;
class UEquipmentSuccessData;
enum class EWeaponType :uint8;
enum class EEquipmentType : uint8;

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
	TObjectPtr<UCurveTable> PlayerLvCurveTable;
	UPROPERTY()
	TObjectPtr<UEquipmentSuccessData> SuccessData;
	UPROPERTY()
	TObjectPtr<UDataTable> MonsterDataTable;
	UPROPERTY()
	TObjectPtr<UCurveTable> MonsterLvCurveTable;
	UPROPERTY()
	TObjectPtr<UDataTable> RegionDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> StageDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> ItemDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> ItemProductDataTable;
	UPROPERTY()
	TObjectPtr<UItemTradeConfig> ItemTradeData;
	UPROPERTY()
	TObjectPtr<UDataTable> DialogDataTable;
	UPROPERTY()
	TObjectPtr<UDataTable> EquipmentUpgradeTable;

public:
	UDataManager();
	
	TObjectPtr<UWeaponConfig> GetWeaponConfig(EWeaponType _type) const;
	float GetPlayerLvCurve(uint32 _lv, const FName& _rowName);
	TObjectPtr<UEquipmentSuccessData> GetSuccessData() const { return SuccessData; }
	struct FMonsterData* GetMonsterData(const FName& _id) const;
	float GetMonsterLvCurve(uint32 _lv, const FName& _rowName);
	struct FRegionData* GetRegionData(const FName& _id) const;
	struct FStageData* GetStageData(const FName& _id) const;
	struct FItemData* GetItemData(const FName& _id) const;
	struct FItemProductData* GetItemProductData(const FName& _id) const;
	void GetAllItemProduct(TArray<struct FItemProductData*>& _outArr) const;
	void GetItemTradeTable(TArray<struct FItemTradeData*>& _outTradeData) const;

	struct FDialogData* GetDialogData(const FName& _id) const;
	struct FEquipmentUpgradeData* GetUpgradeData(uint8 _rank, uint8 _grade, EEquipmentType _type);
};
