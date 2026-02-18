// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemDetail.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemDetail : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TypeLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> EquipmentInfo;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUWStatInfo> StatInfoUIClass;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UVerticalBox> StatInfoContainer;
	
protected:
	void NativeOnInitialized() override;

public:
	void SetDetail(TObjectPtr<class UItem> _item);
};
