// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentUpgrade.generated.h"

class UBorder;
class UButton;
class UScrollBox;
class UHorizontalBox;
class UUWCheckBox;

class UUWListElementSlot;
class UUWIngredientSlot;
class UUWStatChangeCompare;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipmentUpgrade : public UUWPopUp
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditAnywhere)
	uint8 EquipmentSlotInitCount{ 100 };
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWListElementSlot> EquipmentSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWListElementSlot>> EquipmentSlotInst;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> EquipmentSlotContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Result;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false) override;
	void Init();
};
