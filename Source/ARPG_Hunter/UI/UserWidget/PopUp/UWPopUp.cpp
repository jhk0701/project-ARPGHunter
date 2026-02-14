// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/PopUp/UWPopUp.h"

#include "Controller/PlayerCharacterController.h"

void UUWPopUp::ShowUI()
{
	// TODO : 멀티 플레이 시, 주의
	// 리슨 서버 Host 일때는 주의
	if (TObjectPtr<APlayerCharacterController> PC = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController()))
		PC->LockCursor(GetCachedWidget()); // 이 UI로 포커스

	AddToViewport();
}

void UUWPopUp::HideUI()
{
	if (TObjectPtr<APlayerCharacterController> PC = Cast<APlayerCharacterController>(GetWorld()->GetFirstPlayerController()))
		PC->UnLockCursor();

	RemoveFromParent();
}
