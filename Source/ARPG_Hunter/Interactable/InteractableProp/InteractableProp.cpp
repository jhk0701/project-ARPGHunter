// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/InteractableProp/InteractableProp.h"
#include "Components/BoxComponent.h"

AInteractableProp::AInteractableProp()
{
	ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(ColliderComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
