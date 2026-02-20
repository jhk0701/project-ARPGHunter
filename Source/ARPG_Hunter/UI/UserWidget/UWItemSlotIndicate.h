// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/UWItemSlotBase.h"
#include "UWItemSlotIndicate.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemSlotIndicate : public UUWItemSlotBase
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> MessageLabel;

public:
	void SetMessage(const FText& _content);
};
