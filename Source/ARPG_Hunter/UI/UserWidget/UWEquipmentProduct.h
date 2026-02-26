// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentProduct.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UVerticalBox;
class UBorder;

enum class ECharacterStatType : uint8;
enum class EItemType : uint8;

DECLARE_DELEGATE_OneParam(FOnProductSlotClicked, uint8)

UCLASS()
class ARPG_HUNTER_API UUWProductSlot : public UUserWidget
{
	GENERATED_BODY()
private:
	uint8 Index;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SlotButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

protected:
	void NativeOnInitialized() override;

public:
	FOnProductSlotClicked OnProductSlotClicked;

	void SetIndex(uint8 _idx) { Index = _idx; }
	void SetSlot(const FText& _nameText, TObjectPtr<UTexture2D> _thumbnail);

	UFUNCTION()
	void ClickSlot();
};


UCLASS()
class ARPG_HUNTER_API UUWIngredientSlot : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmountLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Thumbnail;

public:
	void SetSlot(const FText& _nameText, const FText& _amountText, TObjectPtr<UTexture2D> _thumbnail);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipmentProduct : public UUWPopUp
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
	
	TArray<struct FItemProductData*> DataArray;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWProductSlot> ProductSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWProductSlot>> ProductSlotInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ProductSlotContainer;
	
	uint8 CurIndex;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemTypeLabel;
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
	TObjectPtr<class UScrollBox> IngredientSlotContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldLabel;

	bool bIngredientIsEnough;
	bool bGoldIsEnough;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ProductButton;

	void ClickProductSlot(uint8 _index);
	void UpdateDetail();

protected:
	void NativeOnInitialized() override;

public:
	void ShowUI(bool _bIsSubUI) override;
	void Init();
};