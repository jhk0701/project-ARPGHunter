// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWEquipmentProduct.generated.h"

class UButton;
class UTextBlock;
class UImage;
class UVerticalBox;

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

	void Init(uint8 _idx);
	void SetSlot(const FText& _nameText, TObjectPtr<UTexture2D> _thumbnail);

	UFUNCTION()
	void ClickSlot();
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWEquipmentProduct : public UUWPopUp
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWProductSlot> SlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWProductSlot>> SlotInst;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> ProductSlotContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

protected:
	void NativeOnInitialized() override;

public:
	void ShowUI(bool _bIsSubUI) override;
	void Init();

};