// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "Types/SlateEnums.h"
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> NewGamePanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UEditableText> NameInputField;
	UPROPERTY(EditAnywhere)
	uint8 MaxNameLength{ 20 };
	FString InputName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackToTitleButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> OverWriteWarningLabel;

	UFUNCTION()
	void ClickNewGame();
	UFUNCTION()
	void InputNameChanged(const FText& _text);
	UFUNCTION()
	void ClickStartGame();
	UFUNCTION()
	void ClickContinue();
	UFUNCTION()
	void CloseNewGamePanel();
	UFUNCTION()
	void ClickExitGame();

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnClickTitleMenuButton OnClickNewGame;
	FOnClickTitleMenuButton OnClickContinue;
	FOnClickTitleMenuButton OnClickExit;

	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr) override;

	void ShowContinueButton(bool _bIsShow);
	const FString& GetInputName() const { return InputName; };
};
