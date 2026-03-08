// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPlayerStatusBar.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class ARPG_HUNTER_API UUWStatEffectSlot : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> Thumbnail;

public:
	void SetTexture(UTexture2D* _tex);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWPlayerStatusBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthLabel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StaminaLabel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> SkillBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWPoolingContainer> StatEffectIndicator;
	UPROPERTY()
	TMap<UObject*, TWeakObjectPtr<UUserWidget>> MapStatEffectSlot;

public:
	void SetHealthBarPercent(uint16 _remain, uint16 _max);
	void SetStaminaBarPercent(uint16 _remain, uint16 _max);
	void SetSkillBarPercent(uint16 _remain, uint16 _max);
	void RegisterStatEffect(UObject* _effectID, UTexture2D* _tex);
	void RemoveStatEffect(UObject* _effectID);
};
