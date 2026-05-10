// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/PropertyDrop/PropertyDrop.h"
#include "Components/BoxComponent.h"

#include "Core/Subsystem/PlayerManager/PlayerManager.h"

APropertyDrop::APropertyDrop()
{
	ColliderComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(ColliderComp);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(ColliderComp);
}

void APropertyDrop::Interact()
{
	Super::Interact();

	UPlayerManager* PM = GetGameInstance()->GetSubsystem<UPlayerManager>();
	
	PM->AddGold(GoldAmount);
	PM->AddExp(ExpAmount);
	for (const FItemDrop& ItemDrop : Items)
		PM->AddItem(ItemDrop.ItemID, ItemDrop.Amount);
}
