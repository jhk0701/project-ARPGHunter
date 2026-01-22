// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/UserWidget/UWPlayerHUD.h"

APlayerHUD::APlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false; 

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerUIFinder(TEXT("/Game/06-UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
	if (PlayerUIFinder.Succeeded())
		PlayerUIWidget = PlayerUIFinder.Class;
}

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerUIWidget)
	{
		PlayerUI = CreateWidget<UUWPlayerHUD>(GetWorld(), PlayerUIWidget);
		if (PlayerUI)
			PlayerUI->AddToViewport();
	}
}
