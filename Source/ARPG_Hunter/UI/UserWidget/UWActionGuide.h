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
	TObjectPtr<class UImage> InputKeyImage;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> ActionNameLabel;

	UPROPERTY(EditAnywhere)
	TMap<EAttackType, TObjectPtr<UTexture2D>> MapTypeInput;

	UPROPERTY(EditAnywhere)
	FText LockText;
	UPROPERTY(EditAnywhere)
	FSlateColor ColorOnLock;
	UPROPERTY(EditAnywhere)
	FSlateColor ColorOnUnlock;

public:
	void SetInfo(EAttackType _type, const FText& _name);
	void SetLocked(EAttackType _type);
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
	void SetActionInfo(bool _bIsInit, int8 _curIdx, const struct FAppliedGraph*);
	void Clear();
};
