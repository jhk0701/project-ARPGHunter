// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWInventory.generated.h"

class UItem;
class UUWItemSlot;
class UUWCheckBox;

enum class EItemType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWInventory : public UUWPopUp
{
	GENERATED_BODY()
private:
	bool bIsSelectMode{ false };
	EItemType CurCategory;
	TFunction<const TArray<TObjectPtr<UItem>>*(EItemType)> GetItemArrFunc;
	uint8 CurSelectedSlot;

	UPROPERTY(EditAnywhere)
	FVector2D SlotSize{80.0f,80.0f};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> CategoryContainer;
	UPROPERTY()
	TMap<EItemType, TObjectPtr<UUWCheckBox>> Category;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWItemSlot> ItemSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWItemSlot>> ItemSlots;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> SlotContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWItemDetail> SelectedItemDetail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> GoldLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> CloseButton;

	void OnSlotClicked(uint8 _index);

protected:
	void NativeOnInitialized() override;

public:
	virtual void ShowUI() override;
	virtual void HideUI() override;
	void ShowUIAsSelectMode(EItemType _itemType);

	void Init(uint8 _initSize, uint32 _gold, TFunction<const TArray<TObjectPtr<UItem>>*(EItemType)> _getItemArrFunc = nullptr);
	bool IsValid() const { return GetItemArrFunc != nullptr; }

	void SetSlot(uint8 _idx, TObjectPtr<UItem> _item);
	void SetGoldLabel(uint32 _goldValue);

	void UpdateSlot();

	UFUNCTION()
	void ClickCloseButton();
	UFUNCTION()
	void ClickCategoryCheckBox(bool _bIsChecked, uint8 _opt);
};
