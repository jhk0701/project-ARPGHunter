// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/StageSelector.h"
#include "Components/BoxComponent.h"

// Sets default values
AStageSelector::AStageSelector()
{
	PrimaryActorTick.bCanEverTick = false;

	ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(ColliderComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(ColliderComp);
}

void AStageSelector::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Interact"));
}
