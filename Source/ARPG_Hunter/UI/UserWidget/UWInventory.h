// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWInventory.generated.h"

class UItem;
class UWrapBox;
class UHorizontalBox;
class UButton;

class UUWItemSlot;
class UUWItemDetail;
enum class EItemType : uint8;

DECLARE_DELEGATE_TwoParams(FOnItemOptionClicked, EItemType, uint8);
DECLARE_DELEGATE_RetVal_OneParam(const TArray<TObjectPtr<UItem>>&, FGetItemArrFunc, EItemType);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWInventory : public UUWPopUp
{
	GENERATED_BODY()
private:
	EItemType CurCategory;
	uint8 CurSelectedSlot;
	int8 OptionalIndex{ -1 }; // 장비, 소비템 장착 시, 자리에 해당하는 인덱스가 들어올 것. (장비 : 타입 열거형 값, 소비템 : 퀵슬롯 인덱스)
	FGetItemArrFunc GetItemArrFunc;

	UPROPERTY(EditAnywhere)
	FVector2D SlotSize{80.0f,80.0f};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWCategory> ItemCategory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWItemSlot> ItemSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWItemSlot>> ItemSlots;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> SlotContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWItemDetail> SelectedItemDetail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWItemDetail> ComparedItemDetail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> ItemOptionContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ThrowButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EquipButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UnequipButton;

	void OnSlotClicked(uint8 _index);
	void ShowSelectedItemDetail(bool _bShow);
	void ClickCategory(uint8 _opt);
	void UpdateCategory(EItemType _category, bool _bUpdateSlot = true);
	void UpdateSlot();

	UFUNCTION()
	void ClickThrowItem();
	UFUNCTION()
	void ClickEquipItem();
	UFUNCTION()
	void ClickUnequipItem();

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr)  override;
	void SetSelectOption(EItemType _itemType, TWeakObjectPtr<UItem> _item, uint8 _optionalIdx);

	virtual void HideUI() override;

	void Init(uint8 _initSize, uint32 _gold, FGetItemArrFunc&& _func);
	bool IsValid() const { return GetItemArrFunc.IsBound(); }
	void SetSlot(uint8 _idx, TWeakObjectPtr<UItem> _item);
	void SetGoldLabel(uint32 _goldValue);
	int8 GetOptionalIndex() const { return OptionalIndex; }

	FOnItemOptionClicked OnThrowButtonClicked;
	FOnItemOptionClicked OnEquipButtonClicked;
	FOnItemOptionClicked OnUnequipButtonClicked;
};
