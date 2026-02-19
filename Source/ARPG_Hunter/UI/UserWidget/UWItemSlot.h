// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemSlot.generated.h"

class UItem;
class UImage;

DECLARE_DELEGATE_OneParam(FOnSlotClicked, uint8);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	uint8 Index;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectedMark;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmountLabel;

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	FOnSlotClicked OnSlotClicked;

	void Init(uint8 _idx, const FVector2D& _size);
	void SetSize(const FVector2D& _size);
	void SetItem(TObjectPtr<UItem> _item);
	void ClearSlot();

	void MarkSelected(bool _bIsSelected);
};
