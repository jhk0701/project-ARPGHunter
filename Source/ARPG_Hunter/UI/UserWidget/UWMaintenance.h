// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWMaintenance.generated.h"

class UWrapBox;
class UUWItemSlot;
class UUWStatInfo;
class UEquipmentItem;
class UConsumableItem;
enum class ECharacterStatType : uint8;
enum class EEquipmentType : uint8;
enum class EItemType : uint8;

struct FUWMaintenanceInitParam 
{
public:
	const TMap<ECharacterStatType, uint32>& PlayerStat;
	const TMap<ECharacterStatType, uint32>& EquipmentStat;
	const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& Equipment;
	const TArray<TObjectPtr<UConsumableItem>>& QuickSlot;
};

DECLARE_DELEGATE_TwoParams(FOnEquipmentSlotClicked, EItemType, uint8);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWMaintenance : public UUWPopUp
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere);
	TMap<EEquipmentType, TObjectPtr<UUWItemSlot>> MapEquipmentSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> WeaponContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> ArmorContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> QuickSlotContainer;
	UPROPERTY()
	TArray<TObjectPtr<UUWItemSlot>> ArrQuickSlot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWStatInfo> StatInfoUIClass;
	UPROPERTY()
	TMap<ECharacterStatType, TObjectPtr<UUWStatInfo>> MapStatInfo;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> StatContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton;

protected:
	void NativeOnInitialized() override;

public:
	FOnEquipmentSlotClicked OnEquipmentSlotClicked;

	void Init(const FUWMaintenanceInitParam& _param);
	void SetStatInfo(const TMap<ECharacterStatType, uint32>& _playerStat, const TMap<ECharacterStatType, uint32>& _equipmentStat);
	void SetEquipment(EEquipmentType _type, TObjectPtr<UEquipmentItem> _equipment);

	UFUNCTION()
	void ClickCloseButton();

	void ClickEquipmentSlot(EItemType _type, uint8 _opt);
};
