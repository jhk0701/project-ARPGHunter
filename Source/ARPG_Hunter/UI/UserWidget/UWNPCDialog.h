// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWNPCDialog.generated.h"

class UTextBlock;

DECLARE_DELEGATE(FOnOptionClicked);
DECLARE_DELEGATE_RetVal_OneParam(TObjectPtr<class UUWPopUp>, FGetUIFunc, UClass*);

UCLASS()
class ARPG_HUNTER_API UUWDialogOption : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonLabel;

protected:
	void NativeOnInitialized() override;

public:
	FOnOptionClicked OnOptionClicked;
	void SetButtonLabel(const FText& _text);

	UFUNCTION()
	void ClickOption();
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWNPCDialog : public UUWPopUp
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogLabel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> DialogOptionContainer;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWDialogOption> DialogOptionClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWDialogOption>> DialogOptionInst;
	UPROPERTY()
	TObjectPtr<UUWDialogOption> CloseOption; // 고정 옵션

public:
	FGetUIFunc GetUIFunc;
	
	void Init(TArray<struct FNPCDialogOption>& _options);
	void SetDialog(struct FDialogData* _firstDialogData);
};
