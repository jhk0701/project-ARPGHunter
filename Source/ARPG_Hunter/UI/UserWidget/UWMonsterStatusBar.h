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

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OpenAnim;

public:
	void SetHealthBarPercent(uint16 _remain, uint16 _max);

	void PlayOpenAnim(bool _bIsReverse = false);
};

UCLASS()
class ARPG_HUNTER_API UUWBossMonsterStatusBar : public UUWMonsterStatusBar
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaggerBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> GimicStaggerContainer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> GimicStaggerBar;

public:
	void SetStaggerBarPercent(uint16 _remain, uint16 _max);
	void SetGimicStaggerBarPercent(uint16 _remain, uint16 _max);
	void ShowUI();
	void HideUI();

	void ShowGimicStagger();
	void HideGimicStagger();
};
