// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "Controller/PlayerCharacterController.h"


void UUWPopUp::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SetIsFocusable(true);
}

FReply UUWPopUp::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsShowing()) 
	{
		if (InKeyEvent.GetKey() == CloseKey ||
			InKeyEvent.GetKey() == AltCloseKey)
		{
			HideUI();
			return FReply::Handled();
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUWPopUp::ShowUI(bool _bIsSubUI, TWeakObjectPtr<UUserWidget> _mainUI)
{
	bIsSubUI = _bIsSubUI;
	if (bIsSubUI && _mainUI.IsValid())
		MainUI = _mainUI;

	// TODO : 멀티 플레이 시, 주의
	// 리슨 서버 Host 일때는 주의
	if (TObjectPtr<APlayerCharacterController> PC = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController()))
		PC->LockCursor(GetCachedWidget()); // 이 UI로 포커스

	if(IsInViewport() == false) 
		AddToViewport(ZOrder);

	SetVisibility(ESlateVisibility::Visible);
	bIsShowing = true;

	// if (IsFocusable())
	SetFocus();
}

void UUWPopUp::HideUI()
{
	// RemoveFromParent();
	SetVisibility(ESlateVisibility::Hidden);

	bIsShowing = false;

	if (bIsSubUI)
	{
		if (MainUI.IsValid())
			MainUI->SetFocus();

		return;
	}

	if (TObjectPtr<APlayerCharacterController> PC = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController()))
		PC->UnLockCursor();
}

void UUWPopUp::RemoveUI()
{
	if (IsInViewport())
		RemoveFromParent();
}
