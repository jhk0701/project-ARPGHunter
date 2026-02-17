// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWEquipment.generated.h"

class UTextBlock;
class UEquipmentItem;
enum class EEquipmentType :uint8;
enum class ECharacterStatType :uint8;

UCLASS()
class ARPG_HUNTER_API UUWEquipmentStatInfo : public UUserWidget 
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueLabel;
public:
	void SetStatName(ECharacterStatType _type);
	void SetStatValue(uint32 _value);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipment : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> SlotContainer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWEquipmentStatInfo> StatInfoUIClass;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> StatContainer;

	UPROPERTY()
	TMap<EEquipmentType, TObjectPtr<class UUWItemSlot>> Slots;
	UPROPERTY()
	TMap<ECharacterStatType, TObjectPtr<UUWEquipmentStatInfo>> StatInfos;

protected:
	virtual void NativeOnInitialized();
	
public:
	void Init(const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& _container, const TMap<ECharacterStatType, uint32>& _equipmentStat);
	void SetSlot(EEquipmentType _type, TObjectPtr<UEquipmentItem> _item);
	void SetStatInfo(const TMap<ECharacterStatType, uint32>& _mapStat);
};
