// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWMaintenance.generated.h"

class UTextBlock;
class UButton;
class UVerticalBox;
class UHorizontalBox;
class UWrapBox;
class UUWItemSlot;
class UEquipmentItem;
class UUWStatInfo;
enum class ECharacterStatType : uint8;
enum class EEquipmentType : uint8;


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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWStatInfo> StatInfoUIClass;
	UPROPERTY()
	TMap<ECharacterStatType, TObjectPtr<UUWStatInfo>> MapStatInfo;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

protected:
	void NativeOnInitialized() override;

public:
	void Init(const TMap<ECharacterStatType, uint32>& _stat, const TMap<ECharacterStatType, uint32>& _equipmentStat, const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& _equipment);
	void SetStatInfo(const TMap<ECharacterStatType, uint32>& _stat, const TMap<ECharacterStatType, uint32>& _equipmentStat);
	void SetEquipment(EEquipmentType _type, TObjectPtr<UEquipmentItem> _equipment);

	UFUNCTION()
	void ClickCloseButton();
};
