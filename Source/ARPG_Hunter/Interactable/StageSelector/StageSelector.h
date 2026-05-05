// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "StageSelector.generated.h"

class UUWStageSelect;

UCLASS()
class ARPG_HUNTER_API AStageSelector : public AInteractableActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> ColliderComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUWStageSelect> StageSelectUIClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UUWStageSelect> StageSelectUI;

public:	
	AStageSelector();
	// Inherited via IInteractable
	virtual void Interact() override;

protected:
	virtual void BeginPlay() override;

private:
	void StartGame(const FName& _selectedID);

};
