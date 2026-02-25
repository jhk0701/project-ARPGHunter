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
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DialogLabel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> DialogOptionContainer;
	UPROPERTY(EditAnywhere)
	uint8 DialogOptionCount{ 10 }; // 대화 옵션 버튼 기본 갯수. 한 NPC에게 10 이상 옵션인 경우 자체가 드물 것으로 생각
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWDialogOption> DialogOptionClass;
	UPROPERTY()
	TArray<TObjectPtr<UUWDialogOption>> DialogOptionInst;
	UPROPERTY()
	TObjectPtr<UUWDialogOption> CloseOption; // 고정 옵션


	void SupplyOptionInst(uint8 _amount);

protected:
	void NativeOnInitialized() override;

public:
	virtual void HideUI() override;
	
	void Init(struct FDialogData* _dialogData, const TArray<struct FNPCDialogOption>& _options);
};
