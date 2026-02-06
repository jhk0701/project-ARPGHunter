// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Actor/DamageFont.h"
#include "Components/WidgetComponent.h"
#include "UI/UserWidget/UWDamageFont.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"

// Sets default values
ADamageFont::ADamageFont()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	SetRootComponent(WidgetComp);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetFinder(TEXT("/Game/06-UI/WBP_DamageFont.WBP_DamageFont_C"));
	if (WidgetFinder.Succeeded())
		WidgetComp->SetWidgetClass(WidgetFinder.Class);

}

void ADamageFont::ShowUI(float _duration)
{
	FTimerManager& Timer = GetWorld()->GetTimerManager();
	if (Timer.IsTimerActive(ShowTimer))
		Timer.ClearTimer(ShowTimer);

	Timer.SetTimer(ShowTimer, this, &ADamageFont::HideUI, _duration);

	if (UUWDamageFont* UI = Cast<UUWDamageFont>(WidgetComp->GetWidget()))
		UI->PlayAnim();
}

void ADamageFont::HideUI()
{
	if (UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>())
		ObjectPool->Release(StaticClass(), this);
}

void ADamageFont::UpdateUI(uint32 _damage, bool _bIsCritial)
{
	if (UUWDamageFont* UI = Cast<UUWDamageFont>(WidgetComp->GetWidget()))
		UI->SetDamage(_damage, _bIsCritial);
}
