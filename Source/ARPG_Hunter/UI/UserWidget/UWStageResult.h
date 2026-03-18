// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWStageResult.generated.h"

class UTextBlock;
class UButton;

DECLARE_DELEGATE(FOnClickReturnButton);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWStageResult : public UUWPopUp
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RewardGoldLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RewardExpLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton;

	UPROPERTY(EditAnywhere)
	FVector2D SlotSize{ 60.0f, 60.0f };
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUWItemSlot> ItemSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWItemSlot>> ItemSlotInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> RewardItemContainer;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenAnim;

protected:
	void NativeOnInitialized() override;

public:
	FOnClickReturnButton OnClickReturnButton;

	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr)  override;
	void Update(bool _bIsClear, const struct FStageData* _stageData);

	UFUNCTION()
	void ClickReturnButton();
};
