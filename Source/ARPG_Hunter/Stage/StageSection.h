// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageSection.generated.h"


UCLASS()
class ARPG_HUNTER_API AStageSection : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Setting")
	uint8 Id{0};

	UPROPERTY(EditAnywhere, Category = "Setting")
	bool bIsCleared{false};

	UPROPERTY(EditAnywhere, Category = "Setting")
	TObjectPtr<class UBoxComponent> BoxComp;


public:	
	// Sets default values for this actor's properties
	AStageSection();
	void Init();

protected:
	virtual void BeginPlay() override;
	void SpawnMonster();
};
