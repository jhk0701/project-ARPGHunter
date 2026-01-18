// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/StatComponent.h"

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

}
