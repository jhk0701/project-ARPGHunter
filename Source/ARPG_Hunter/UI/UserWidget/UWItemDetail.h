// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemDetail.generated.h"

class UTextBlock;
class UVerticalBox;
class UUWStatInfo;
enum class ECharacterStatType :uint8;

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
	TObjectPtr<UTextBlock> RankLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> ConsumableInfo;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AmountLabel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UWrapBox> EquipmentInfo;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWStatInfo> StatInfoUIClass;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVerticalBox> StatInfoContainer;
	UPROPERTY()
	TMap<ECharacterStatType, TObjectPtr<UUWStatInfo>> MapStatInfo;

protected:
	void NativeOnInitialized() override;

public:
	void SetDetail(TWeakObjectPtr<class UItem> _item);
	void SetDetail(struct FItemData* _data);
};
