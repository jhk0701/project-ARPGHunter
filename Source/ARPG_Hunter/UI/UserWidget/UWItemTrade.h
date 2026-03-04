// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWItemTrade.generated.h"

class UButton;
class UPanelWidget;

class UUWItemDetail;
class UUWItemSlotIndicate;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemTrade : public UUWPopUp
{
	GENERATED_BODY()

private:
	TArray<struct FItemTradeData*> ItemTradeDatas;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;

	UPROPERTY(EditAnywhere)
	FVector2D SlotSize{ 150.0f, 150.0f };
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWItemSlotIndicate> ItemSlotClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWItemSlotIndicate>> ItemSlotInst;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPanelWidget> ItemContainer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWItemDetail> SelectedItemDetail;

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI) override;
	void Init();
};
