// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWTitleScreen.generated.h"

class UButton;

DECLARE_DELEGATE(FOnClickTitleMenuButton);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWTitleScreen : public UUWPopUp
{
	GENERATED_BODY()

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> NewGameButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ContinueButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

public:
	FOnClickTitleMenuButton OnClickNewGame;
	FOnClickTitleMenuButton OnClickContinue;
	FOnClickTitleMenuButton OnClickExit;

	void ShowContinueButton(bool _bIsShow);
};
