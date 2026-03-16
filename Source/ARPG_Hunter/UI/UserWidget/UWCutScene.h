// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWCutScene.generated.h"

DECLARE_DELEGATE(FOnSkipClicked)

UCLASS()
class ARPG_HUNTER_API UUWCutScene : public UUWPopUp
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButton> SkipButton;

	UFUNCTION()
	void ClickSkip();

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnSkipClicked OnSkipClicked;

	void HideUI() override;

};
