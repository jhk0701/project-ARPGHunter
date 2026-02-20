// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWQuickSlot.generated.h"

class UConsumableItem;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWQuickSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UHorizontalBox> SlotContainer;

	UPROPERTY()
	TArray<TObjectPtr<class UUWItemSlotIndicate>> QuickSlots;

protected:
	void NativeOnInitialized() override;

public:
	void Init(const TArray<TWeakObjectPtr<UConsumableItem>>& _container);
	void SetQuickSlot(uint8 _index, TWeakObjectPtr<UConsumableItem> _item);
};
