// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWStageSelect.generated.h"

class UTextBlock;
class UButton;
class UWrapBox;
class UUWItemSlot;
class UScrollBox;
class UUWStageSlot;

DECLARE_DELEGATE_OneParam(FOnClickStartButton, const FName&);
DECLARE_DELEGATE_OneParam(FOnClickStageSlot, uint8);


UCLASS()
class ARPG_HUNTER_API UUWStageSlot : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	uint8 Index{0};

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SelectButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameLabel;

	UFUNCTION()
	void ClickStageButton();

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnClickStageSlot OnClickStageSlot;
	void Init(uint8 _index);
	void Update(struct FStageData* _data);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWStageSelect : public UUWPopUp
{
	GENERATED_BODY()

private:
#pragma region Widget
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
	TObjectPtr<UTextBlock> ExpRewardLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> RewardItemContainer;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWItemSlot> ItemSlotClass;
	UPROPERTY(EditAnywhere)
	uint8 InitRewardItemSlotSize = 24;
	UPROPERTY(EditAnywhere)
	FVector2D ItemSlotSize{ 50.0f, 50.0f };

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> StageList;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWStageSlot> StageSlotClass;
	UPROPERTY(EditAnywhere)
	uint8 InitStageSlotSize = 10;

	// 직렬화하지 않고 휘발처리
	// BindWidget과 달리 BindWidgetAnim라서 별도 처리
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenAnim;
#pragma endregion

	FName CurRegionID;
	FName CurStageID;

	// 슬롯
	UPROPERTY()
	TArray<TObjectPtr<UUWItemSlot>> ItemSlotContainer;
	UPROPERTY()
	TArray<TObjectPtr<UUWStageSlot>> StageSlotContainer;

public:
	FOnClickStartButton OnClickStartButton;

	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr)  override;

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void ClickStartButton();
	UFUNCTION()
	void ClickStageSlot(uint8 _index);

	void RefreshStageSlot();
	void RefreshStageInfo();
	void Clear();
};
