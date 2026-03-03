// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentSuccession.generated.h"

class UButton;
class UScrollBox;

class UUWEquipmentUtilSlot;
class UUWListElementSlot;

enum class EItemType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipmentSuccession : public UUWPopUp
{
	GENERATED_BODY()

private:
	EItemType CurItemType;
	uint8 EquipmentIdx;
	uint8 IngredientIdx;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWCategory> ItemCategory;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWListElementSlot> EquipmentSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWListElementSlot>> EquipmentSlotInst;
	UPROPERTY()
	TArray<TObjectPtr<UUWListElementSlot>> IngredientSlotInst;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> EquipmentSlotContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> IngredientSlotContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWEquipmentUtilSlot> SuccessedSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWEquipmentUtilSlot> IngredientSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWEquipmentUtilSlot> ResultSlot;
	
	void SelectCategory(uint8 _opt);

	void ClickEquipmentSlot(uint8 _index);
	void ClickIngredientSlot(uint8 _index);

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI) override;
	void Init();
};
