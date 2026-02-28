// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWCategory.generated.h"


DECLARE_DELEGATE_OneParam(FOnOptionSelected, uint32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnCategorySelected, uint32);

UCLASS()
class ARPG_HUNTER_API UUWCategoryElement : public UUserWidget 
{
	GENERATED_BODY()

private:
	uint32 Index;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UCheckBox> CheckBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> Label;

	UFUNCTION()
	void OnStateChanged(bool _bIsOn);

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnOptionSelected OnSelected;
	void Init(uint32 _index, const FText& _text);
	
	void SetSelectedManually(bool _bIsOn);
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
	uint32 CurSelectedOption;

	UPROPERTY(EditAnywhere)
	TArray<FCategoryOption> CategoryOptions;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWCategoryElement> ElementClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWCategoryElement>> ElementInst;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPanelWidget> Container;

	void OnElementSelected(uint32 _value);

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnCategorySelected OnSelected;
	uint32 GetSelectedOption() const { return CurSelectedOption; }
};
