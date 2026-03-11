// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentProduct.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UBorder;
class UVerticalBox;
class UScrollBox;

class UUWListElementSlot;
class UUWIngredientSlot;

enum class ECharacterStatType : uint8;
enum class EItemType : uint8;


UCLASS()
class ARPG_HUNTER_API UUWEquipmentProduct : public UUWPopUp
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
	
	TArray<struct FItemProductData*> DataArray;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWListElementSlot> ProductSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWListElementSlot>> ProductSlotInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ProductSlotContainer;
	
	uint8 CurIndex;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemTypeLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemRankLabel;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUWStatInfo> StatInfoUIClass;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatInfoContainer;
	UPROPERTY()
	TMap<ECharacterStatType, TObjectPtr<UUWStatInfo>> MapStatInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> IngredientDetail;
	UPROPERTY(EditAnywhere)
	uint8 InitIngredientSlotCount{ 10 }; // 초기 슬롯 생성 개수
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWIngredientSlot> IngredientSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWIngredientSlot>> IngredientSlotInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> IngredientSlotContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWIngredientSlot> GoldSlot;

	bool bIngredientIsEnough;
	bool bGoldIsEnough;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ProductButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ProductResult;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ProductItemThumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ProductItemLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CompleteButton;

	void ClickProductSlot(uint8 _index);
	void UpdateDetail();

	UFUNCTION()
	void ClickProductButton();
	UFUNCTION()
	void ClickCompleteButton();

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;
	void Init();
};