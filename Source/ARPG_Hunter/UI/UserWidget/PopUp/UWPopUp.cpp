// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/PopUp/UWPopUp.h"
#include "Controller/PlayerCharacterController.h"

FReply UUWPopUp::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsShowing()) 
	{
		if (InKeyEvent.GetKey() == EKeys::Escape ||
			InKeyEvent.GetKey() == CloseKey)
		{
			HideUI();
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UUWPopUp::ShowUI(bool _bIsSubUI)
{
	bIsSubUI = _bIsSubUI;

	// TODO : 멀티 플레이 시, 주의
	// 리슨 서버 Host 일때는 주의
	if (TObjectPtr<APlayerCharacterController> PC = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController()))
		PC->LockCursor(GetCachedWidget()); // 이 UI로 포커스

	AddToViewport();
	bIsShowing = true;

	if (IsFocusable())
		SetFocus();
}

void UUWPopUp::HideUI()
{
	RemoveFromParent();
	bIsShowing = false;

	if (bIsSubUI)
		return;

	if (TObjectPtr<APlayerCharacterController> PC = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController()))
		PC->UnLockCursor();
}
