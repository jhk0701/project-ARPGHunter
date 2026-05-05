// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "InteractableActor.generated.h"

UCLASS(Abstract)
class ARPG_HUNTER_API AInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	FText NameText;

public:
	// IInteractable을(를) 통해 상속됨
	virtual void Interact() override {};
	virtual const FText& GetInteractName() override { return NameText; };
};
