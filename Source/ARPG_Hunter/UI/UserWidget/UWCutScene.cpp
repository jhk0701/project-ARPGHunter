// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/UWCutScene.h"
#include "Components/Button.h"


void UUWCutScene::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SkipButton->OnClicked.AddDynamic(this, &UUWCutScene::ClickSkip);
}

void UUWCutScene::HideUI()
{
	Super::HideUI();
	RemoveUI(); // 컷씬은 재사용될 가능성이 낮으므로 뷰포트에서 바로 제거
}

void UUWCutScene::ClickSkip()
{
	OnSkipClicked.ExecuteIfBound();
	HideUI();
}
