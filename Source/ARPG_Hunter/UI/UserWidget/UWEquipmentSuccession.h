// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentSuccession.generated.h"

class UButton;
class UScrollBox;

class UUWEquipmentUtilSlot;
class UUWListElementSlot;
class UUWIngredientSlot;

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
	bool bSuccessionIsEnable;

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
	TObjectPtr<UUWIngredientSlot> GoldSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SuccessButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> Result;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWEquipmentUtilSlot> ResultSlot;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;

	
	void SelectCategory(uint8 _opt);

	void ClickEquipmentSlot(uint8 _index);
	void ClickIngredientSlot(uint8 _index);

	UFUNCTION()
	void ConfirmResult();
	UFUNCTION()
	void SucceessItem();

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;
	void Init();
};
