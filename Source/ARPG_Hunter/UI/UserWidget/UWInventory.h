// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWInventory.generated.h"

class UItem;
class UUWItemSlot;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWInventory : public UUWPopUp
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
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton;

protected:
	void NativeOnInitialized() override;

public:
	void Init(uint8 _initSize, uint32 _gold);
	void SetSlot(uint8 _idx, TObjectPtr<UItem> _item);
	void SetGoldLabel(uint32 _goldValue);

	UFUNCTION()
	void ClickCloseButton();
};
