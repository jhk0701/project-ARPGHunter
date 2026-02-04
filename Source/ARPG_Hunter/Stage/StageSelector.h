// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "StageSelector.generated.h"

class UUWStageSelect;

UCLASS()
class ARPG_HUNTER_API AStageSelector : public AActor, public IInteractable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> ColliderComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUWStageSelect> StageSelectUIClass;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUWStageSelect> StageSelectUI;

public:	
	AStageSelector();
	// Inherited via IInteractable
	void Interact() override;

protected:
	void BeginPlay() override;

};
