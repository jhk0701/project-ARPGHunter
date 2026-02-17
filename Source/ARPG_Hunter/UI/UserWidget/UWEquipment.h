// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWEquipment.generated.h"

class UEquipmentItem;
enum class EEquipmentType :uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipment : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UUniformGridPanel> SlotContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> StatContainer;

	UPROPERTY()
	TMap<EEquipmentType, TObjectPtr<class UUWItemSlot>> Slots;

protected:
	virtual void NativeOnInitialized();
	
public:
	void Init(const TMap<EEquipmentType, TObjectPtr<UEquipmentItem>>& _container);
	void SetSlot(EEquipmentType _type, TObjectPtr<UEquipmentItem> _item);
};
