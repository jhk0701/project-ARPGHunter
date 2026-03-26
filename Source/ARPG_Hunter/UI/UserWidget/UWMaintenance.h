// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWMaintenance.generated.h"

class UWrapBox;
class UTextBlock;
class UUWItemSlot;
class UUWStatInfo;
class UItem;
class UEquipmentItem;
class UConsumableItem;
enum class ECharacterStatType : uint8;
enum class EEquipmentType : uint8;
enum class EItemType : uint8;

struct FUWMaintenanceInitParam 
{
public:
	const uint16 Level;
	const int32 Exp;
	const int32 ReqExp;
	const TMap<ECharacterStatType, uint32>& PlayerStat;
	const TMap<ECharacterStatType, uint32>& EquipmentStat;
	const TMap<EEquipmentType, TWeakObjectPtr<UEquipmentItem>>& Equipment;
	const TArray<TWeakObjectPtr<UConsumableItem>>& QuickSlot;
};

DECLARE_DELEGATE_TwoParams(FOnEquipmentSlotClicked, EItemType, uint8);
DECLARE_DELEGATE_OneParam(FOnQuickSlotClicked, uint8);

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ExpLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ExpBar;

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
	FOnQuickSlotClicked OnQuickSlotClicked;

	void Init(const FUWMaintenanceInitParam& _param);
	void SetLevelInfo(uint16 _level, int32 _exp, int32 _reqExp);
	void SetStatInfo(const TMap<ECharacterStatType, uint32>& _playerStat, const TMap<ECharacterStatType, uint32>& _equipmentStat);
	void SetEquipment(EEquipmentType _type, TWeakObjectPtr<UEquipmentItem> _equipment);
	void SetQuickSlot(uint8 _index, TWeakObjectPtr<UConsumableItem> _consumable);
	void OnInventoryChanged(uint8 _idx, TWeakObjectPtr<UItem> _item);

	UFUNCTION()
	void ClickCloseButton();
	void ClickEquipmentSlot(EItemType _type, uint8 _opt);
	void ClickQuickSlot(uint8 _idx);
};
