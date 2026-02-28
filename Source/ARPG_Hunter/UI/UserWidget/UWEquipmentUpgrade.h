// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentUpgrade.generated.h"

class UBorder;
class UButton;
class UImage;
class UTextBlock;
class UScrollBox;
class UHorizontalBox;

class UUWListElementSlot;
class UUWIngredientSlot;
class UUWStatChangeCompare;

enum class EItemType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipmentUpgrade : public UUWPopUp
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<class UInventory> Inventory;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;

	EItemType CurItemType;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWCategory> ItemCategory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWListElementSlot> EquipmentSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWListElementSlot>> EquipmentSlotInst;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> EquipmentSlotContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurGradeLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NextGradeLabel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Result;

	void SelectCategory(uint8 _option);
	void SelectSlot(uint8 _index);

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false) override;
	void Init();
};
