// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/InteractableActor.h"
#include "Components/WidgetComponent.h"
#include "UI/UserWidget/UWInteractionIndicator.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}
