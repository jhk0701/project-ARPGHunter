// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWInventory.generated.h"

class UUWItemSlot;
class UItem;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWInventory : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	FVector2D SlotSize{80.0f,80.0f};

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWItemSlot> ItemSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWItemSlot>> ItemSlots;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> SlotContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldLabel;

public:
	void Init(const TArray<TObjectPtr<UItem>>& _items, uint32 _gold);
	void SetSlot(uint8 _idx, TObjectPtr<UItem> _item);
	void SetGoldLabel(uint32 _goldValue);
};
