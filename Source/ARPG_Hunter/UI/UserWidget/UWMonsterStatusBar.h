// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWMonsterStatusBar.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWMonsterStatusBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaggerBar;

public:
	void SetHealthBarPercent(uint16 _remain, uint16 _max);
	void SetStaggerBarPercent(uint16 _remain, uint16 _max);
};
