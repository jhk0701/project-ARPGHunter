// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemSlot.generated.h"

class UItem;
/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USizeBox> Frame;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Thumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AmountLabel;
public:
	void SetSize(const FVector2D& _size);
	void SetItem(TObjectPtr<UItem> _item);
	void ClearSlot();
};
