// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWActionGuide.generated.h"

class UTextBlock;
enum class EAttackType : uint8;

UCLASS()
class ARPG_HUNTER_API UUWActionInfo : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> InputLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ActionNameLabel;
public:
	void SetInfo(const FText& _input, const FText& _name);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWActionGuide : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UPanelWidget> NextActionContainer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWActionInfo> ActionInfoClass;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUWActionInfo> CurAction;
	UPROPERTY()
	TArray<TObjectPtr<UUWActionInfo>> NextActions;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetActionInfo(bool _bIsInit, uint8 _curIdx, TWeakObjectPtr<class UActionComboData> _comboData);
	void Clear();
};
