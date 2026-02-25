// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWNPCDialog.generated.h"

class UTextBlock;

DECLARE_DELEGATE(FOnOptionClicked)

UCLASS()
class ARPG_HUNTER_API UUWDialogOption : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> Button;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonLabel;

public:
	FOnOptionClicked OnOptionClicked;
	void SetButtonLabel(const FText& _text);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWNPCDialog : public UUWPopUp
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogLabel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> DialogOptionContainer;
	UPROPERTY(EditAnywhere)
	uint8 DialogOptionCount{ 10 };
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWDialogOption> DialogOptionClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWDialogOption>> DialogOptionInst;

	void SupplyOptionInst(uint8 _amount);

protected:
	void NativeOnInitialized() override;

public:
	void SetDialogOption(const TArray<struct FNPCDialog>& _options);
};
