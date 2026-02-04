// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemSlot : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Thumbnail;

public:
	// void Update(struct FItemData* ItemData);
};
