// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actor/DamageFont.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADamageFont::ADamageFont()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("/Game/06-UI/WBP_DamageFont.WBP_DamageFont_C"));
	if (WidgetFinder.Succeeded())
		WidgetComp->SetWidgetClass(WidgetFinder.Class);

}