// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "PropertyDrop.generated.h"


USTRUCT()
struct FItemDrop
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ItemID;
	UPROPERTY(EditAnywhere)
	uint16 Amount{100};
};

UCLASS()
class ARPG_HUNTER_API APropertyDrop : public AInteractableActor
{
	GENERATED_BODY()
public:
	APropertyDrop();
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> ColliderComp;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category="Property")
	uint32 GoldAmount{ 0 };
	UPROPERTY(EditAnywhere, Category = "Property")
	uint32 ExpAmount{ 0 };
	UPROPERTY(EditAnywhere, Category = "Property")
	TArray<FItemDrop> Items;

public:
	virtual void Interact() override;

};
