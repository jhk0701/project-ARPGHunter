// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/UWItemSlotBase.h"
#include "UWItemSlot.generated.h"

DECLARE_DELEGATE_OneParam(FOnSlotClicked, uint8);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemSlot : public UUWItemSlotBase
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectedMark;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EquippedMark;

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	FOnSlotClicked OnSlotClicked;

	virtual void SetItem(TWeakObjectPtr<UItem> _item) override;
	virtual void ClearSlot() override;

	void MarkSelected(bool _bIsSelected);
	void MarkEquipped(bool _bIsEquipped);
	bool IsEquipped() const;
};