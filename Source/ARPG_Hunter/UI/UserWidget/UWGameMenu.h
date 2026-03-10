// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "UWGameMenu.generated.h"

DECLARE_DELEGATE(FOnMenuButtonClicked);

class UButton;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWGameMenu : public UUWPopUp
{
	GENERATED_BODY()

public:
	enum EButtonOption : uint8
	{
		EXIT,
		RETURN,
	};
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CloseButton;

	// 옵션이 두개 밖에 없어서 바로 바인딩
	// 더 늘어날 것 같으면 별개 UserWidget으로 분리해서 동적생성할 것
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton;

	UFUNCTION()
	void ClickExit();
	UFUNCTION()
	void ClickReturn();

protected:
	virtual void NativeOnInitialized() override;

public:
	FOnMenuButtonClicked OnExitClicked;
	FOnMenuButtonClicked OnReturnClicked;
	
	void ToggleOptionalButton(EButtonOption _opt, bool _bIsOn);
};
