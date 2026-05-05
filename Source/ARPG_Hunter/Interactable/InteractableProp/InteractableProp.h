// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "InteractableProp.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API AInteractableProp : public AInteractableActor
{
	GENERATED_BODY()

public:
	AInteractableProp();

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> ColliderComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

public:


};
