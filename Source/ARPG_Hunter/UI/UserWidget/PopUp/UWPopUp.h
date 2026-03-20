// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWPopUp.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UUWPopUp : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	bool bIsSubUI{ false };
	UPROPERTY(EditAnywhere)
	int32 ZOrder{ 0 };

	TWeakObjectPtr<UUserWidget> MainUI{ nullptr };

	UPROPERTY(VisibleAnywhere)
	bool bIsShowing{ false };

	UPROPERTY(EditAnywhere)
	FKey CloseKey{ EKeys::Escape };

	UPROPERTY(EditAnywhere)
	FKey AltCloseKey{ EKeys::Invalid };

protected:
	bool IsSubUI() const { return bIsSubUI; }

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeOnInitialized() override;
public:
	UFUNCTION()
	virtual void ShowUI(bool _bIsSubUI = false, TWeakObjectPtr<UUserWidget> _mainUI = nullptr);
	UFUNCTION()
	virtual void HideUI();
	UFUNCTION()
	virtual void RemoveUI();

	bool IsShowing() const { return bIsShowing; }
};

