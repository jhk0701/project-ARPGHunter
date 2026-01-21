// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

private:
	void PlayBT(APawn* _inPawn);

public:
	AMonsterAIController();
	virtual void OnPossess(APawn* InPawn) override;
};
