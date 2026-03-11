// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWInputGuide.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWInputGuide : public UUWPopUp
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UWidgetSwitcher> Pages;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> PrevButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> NextButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUWCategory> PageCategory;
	
	void ClickPageCategory(uint8 _idx);
	UFUNCTION()
	void ClickPrev();
	UFUNCTION()
	void ClickNext();

protected:
	virtual void NativeOnInitialized() override;

public:
	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;
};
