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

public:
	AMonsterAIController();
private:
	void PlayBT(APawn* _inPawn);
protected:
	virtual void OnPossess(APawn* InPawn) override;
public:
	void StopBT();
	void RestartBT();

};
