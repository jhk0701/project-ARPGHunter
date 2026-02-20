// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemSlotBase.generated.h"

class UItem;
class UImage;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemSlotBase : public UUserWidget
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
	TObjectPtr<class UTextBlock> AmountLabel;

protected:
	uint8 GetIndex() const { return Index; }

public:
	void Init(uint8 _idx);
	void Init(uint8 _idx, const FVector2D& _size);

	void SetSize(const FVector2D& _size);
	virtual void SetItem(TWeakObjectPtr<UItem> _item);
	virtual void ClearSlot();
};
