// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Containers/Queue.h"
#include "ObjectPoolManager.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UObjectPoolManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UObjectPoolManager();

private:
	UPROPERTY()
	TQueue<TObjectPtr<AActor>> Pool;
	

};
