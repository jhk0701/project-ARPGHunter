// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemSlot.generated.h"

class UItem;
class UImage;

DECLARE_DELEGATE_OneParam(FSlotEvent, uint8);

/**
 *
 */
UCLASS(Abstract)
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
	void SetSize(const FVector2D& _size);
	void SetAmount(uint32 _amount);

	virtual void SetSlot(TWeakObjectPtr<UItem> _item);
	virtual void SetSlotUsingID(const FName& _id, uint32 _amount);
	virtual void SetThumbnailOnly(TObjectPtr<UTexture2D> _tex);
	virtual void ClearSlot();
};

UCLASS()
class ARPG_HUNTER_API UUWSelectableItemSlot : public UUWItemSlotBase 
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SelectedMark;

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	FSlotEvent OnSlotClicked;
	FSlotEvent OnSlotHovered;

	void MarkSelected(bool _bIsSelected);
};

UCLASS()
class ARPG_HUNTER_API UUWItemSlot : public UUWSelectableItemSlot
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> EquippedMark;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void SetSlot(TWeakObjectPtr<UItem> _item) override;
	virtual void SetThumbnailOnly(TObjectPtr<UTexture2D> _tex) override;
	virtual void ClearSlot() override;

	void MarkEquipped(bool _bIsEquipped);
	bool IsEquipped() const;
};


UCLASS()
class ARPG_HUNTER_API UUWItemSlotIndicate : public UUWItemSlotBase
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageLabel;

public:
	void SetMessage(const FText& _content);
};

UCLASS()
class ARPG_HUNTER_API UUWItemTradeSlot : public UUWSelectableItemSlot
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PriceLabel;

public:
	void SetPrice(uint32 _price);
};
