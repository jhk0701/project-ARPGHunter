// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWCategory.generated.h"


DECLARE_DELEGATE_OneParam(FOnOptionSelected, uint8);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCategorySelected, uint8);

UCLASS()
class ARPG_HUNTER_API UUWCategoryElement : public UUserWidget 
{
	GENERATED_BODY()

private:
	uint8 Index;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCheckBox> CheckBox;

	UFUNCTION()
	void OnStateChanged(bool _bIsOn);

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnOptionSelected OnSelected;
	virtual void Init(uint8 _index, const FText& _text);
	
	void SetSelectedManually(bool _bIsOn);
};

UCLASS()
class ARPG_HUNTER_API UUWCategoryLabeledElement : public UUWCategoryElement
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Label;

public:
	virtual void Init(uint8 _index, const FText& _text) override;
};

USTRUCT()
struct FCategoryOption 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint32 Value;
	UPROPERTY(EditAnywhere)
	FText Text;
};

UCLASS()
class ARPG_HUNTER_API UUWCategory : public UUserWidget
{
	GENERATED_BODY()

private:
	uint8 CurSelectedOption;

	UPROPERTY(EditAnywhere)
	TArray<FCategoryOption> CategoryOptions;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWCategoryElement> ElementClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWCategoryElement>> ElementInst;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> Container;

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnCategorySelected OnSelected;
	uint8 GetSelectedOption() const { return CurSelectedOption; }
	void SetSelectedElement(uint8 _value);
	void SetSelectElementManually(uint8 _value);
	void SetPrevElement();
	void SetNextElement();
};
