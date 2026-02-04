// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWStageResult.generated.h"

class UTextBlock;
class UButton;

DECLARE_DELEGATE(FOnClickReturnButton);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWStageResult : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ResultLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RewardGoldLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenAnim;

public:
	FOnClickReturnButton OnClickReturnButton;

	void ShowUI();
	void HideUI();

	void Update(bool _bIsClear);

	UFUNCTION()
	void ClickReturnButton();

protected:
	void NativeOnInitialized() override;
};
