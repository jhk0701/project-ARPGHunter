// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWItemDetail.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWItemDetail : public UUserWidget
{
	GENERATED_BODY()
private:

public:
	void SetDetail(TObjectPtr<class UItemConfig> _config);
};
