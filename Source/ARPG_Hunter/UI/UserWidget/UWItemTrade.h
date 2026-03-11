// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWItemTrade.generated.h"

class UButton;
class UPanelWidget;

class UUWItemDetail;
class UUWItemTradeSlot;
class UUWIngredientSlot;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemTrade : public UUWPopUp
{
	GENERATED_BODY()

private:
	TArray<struct FItemTradeData*> ItemTradeDatas;
	TArray<uint32> RemainAmount;

	uint8 SelectedIndex{0};
	uint32 InputAmount{0};

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditAnywhere)
	FVector2D SlotSize{ 150.0f, 150.0f };
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWItemTradeSlot> ItemSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWItemTradeSlot>> ItemSlotInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ItemContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWItemDetail> SelectedItemDetail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWIngredientSlot> PriceSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> AmountInputField;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> Option;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SubTenButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SubOneButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AddOneButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AddTenButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PurchaseButton;

	void ClickSlot(uint8 _idx);
	void UpdatePriceSlot();

	UFUNCTION()
	void Purchase();
	UFUNCTION()
	void HandleTextEditEvent(const FText& Text);
	UFUNCTION()
	void ClickSubOne();
	UFUNCTION()
	void ClickSubTen();
	UFUNCTION()
	void ClickAddOne();
	UFUNCTION()
	void ClickAddTen();

	void InitOption();

	void ChangeAmount(int32 _amount);
	void SetAmount(uint32 _amount);
	void SetTextWithoutEvent(const FText& _text);

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;
	void Init();
};