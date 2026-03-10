// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/UserWidget/PopUp/UWPopUp.h"


APlayerHUD::APlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false; 

	static ConstructorHelpers::FClassFinder<UUWPopUp> InputGuideClassFinder(TEXT("/Game/06-UI/WBP_InputGuide.WBP_InputGuide_C"));
	if (InputGuideClassFinder.Succeeded())
		InputGuideUIClass = InputGuideClassFinder.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (InputGuideUIClass) 
	{
		InputGuideUI = CreateWidget<UUWPopUp>(GetWorld(), InputGuideUIClass);
	}
}

void APlayerHUD::ToggleInputGuideUI()
{
	if (nullptr == InputGuideUI)
		return;

	if (InputGuideUI->IsShowing())
		InputGuideUI->HideUI();
	else 
		InputGuideUI->ShowUI();
}
