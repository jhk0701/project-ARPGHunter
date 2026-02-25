// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPC.generated.h"

UCLASS()
class ARPG_HUNTER_API ANPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UNPCConfig> Config;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
