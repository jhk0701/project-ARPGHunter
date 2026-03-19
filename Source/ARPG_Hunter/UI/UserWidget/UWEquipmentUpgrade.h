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
class UVerticalBox;
class UWrapBox;

class UUWListElementSlot;
class UUWIngredientSlot;
class UUWStatChangeCompare;

enum class EItemType : uint8;
enum class EEquipmentType : uint8;
enum class ECharacterStatType : uint8;

// Detail 창에 표시하기 위함
UENUM() 
enum class EDisableReason : uint8
{
	NO_UPGRADE_DATA,
	IS_MAX_UPGRADE,
	SELECT_ITEM,

	NONE
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipmentUpgrade : public UUWPopUp
{
	GENERATED_BODY()
	
private:
	EItemType CurItemType;
	uint8 CurItemIdx;
	struct FEquipmentUpgradeData* UpgradeData;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWCategory> ItemCategory;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWListElementSlot> EquipmentSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWListElementSlot>> EquipmentSlotInst;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UScrollBox> EquipmentSlotContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> UpgradeBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurGradeLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NextGradeLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SucceessPercentLabel;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWStatChangeCompare> StatChangeClass;
	UPROPERTY()
	TMap<ECharacterStatType, TObjectPtr<UUWStatChangeCompare>> StatChangeInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> UpgradeStatContainer;

	UPROPERTY(EditAnywhere)
	uint8 InitIngredientSlotSize{ 10 };
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWIngredientSlot> IngredientSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWIngredientSlot>> IngredientSlotInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> IngredientInfo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWIngredientSlot> GoldSlot;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> UpgradeButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Result;

	UPROPERTY(EditAnywhere)
	FText TextOnSuccess;
	UPROPERTY(EditAnywhere)
	FText TextOnFail;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ResultLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ThumbnailOnResult;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemLabelOnResult;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ConfirmButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DisableLabel;
	UPROPERTY(EditAnywhere)
	TMap<EDisableReason, FText> DisableReasonText;

	void SelectCategory(uint8 _option);
	void SelectSlot(uint8 _index);
	void ShowUpgradeDetail(bool _bIsEnable, EDisableReason _reason = EDisableReason::NONE);
	UFUNCTION()
	void Upgrade();
	UFUNCTION()
	void ConfirmResult();
	void ShowResult(bool _bIsSuccess);

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr)  override;
	void Init();
};
