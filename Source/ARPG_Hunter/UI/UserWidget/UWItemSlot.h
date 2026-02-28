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

	virtual void SetSlot(TWeakObjectPtr<UItem> _item);
	virtual void SetThumbnailOnly(TObjectPtr<UTexture2D> _tex);
	virtual void ClearSlot();
};


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

	virtual void SetSlot(TWeakObjectPtr<UItem> _item) override;
	virtual void SetThumbnailOnly(TObjectPtr<UTexture2D> _tex) override;
	virtual void ClearSlot() override;

	void MarkSelected(bool _bIsSelected);
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

