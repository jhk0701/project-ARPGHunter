// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerHUD.h"
#include "UI/UserWidget/UWPlayerHUD.h"
#include "Subsystem/ObjectPool/ObjectPoolManager.h"
#include "UI/Actor/DamageFont.h"

APlayerHUD::APlayerHUD()
{
	PrimaryActorTick.bCanEverTick = false; 

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerUIFinder(TEXT("/Game/06-UI/WBP_PlayerHUD.WBP_PlayerHUD_C"));
	if (PlayerUIFinder.Succeeded())
		PlayerUIWidget = PlayerUIFinder.Class;

	static ConstructorHelpers::FClassFinder<AActor> DamageUIFinder(TEXT("/Game/02-BP/DamageFont/BP_DamageFont.BP_DamageFont_C"));
	if (DamageUIFinder.Succeeded())
		DamageUIClass = DamageUIFinder.Class;
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

	if (DamageUIClass)
	{
		UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
		ObjectPool->Register(ADamageFont::StaticClass(), [this]() { return GetWorld()->SpawnActor(DamageUIClass); }, 10);
	}
}
