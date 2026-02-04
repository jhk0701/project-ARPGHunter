// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWStageSelect.generated.h"

class UTextBlock;
class UButton;
class UWrapBox;
class UUWItemSlot;
class UScrollBox;
class UUWStageSlot;

UCLASS()
class ARPG_HUNTER_API UUWStageSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameLabel;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWStageSelect : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageDescLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GoldRewardLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> RewardItemContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWItemSlot> RewardItemSlot;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> StageList;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWStageSlot> StageSlot;

};
