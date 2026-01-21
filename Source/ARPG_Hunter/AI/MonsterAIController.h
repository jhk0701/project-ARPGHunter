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
	void GetAssetFromMonster();
	void PlayBT(TObjectPtr<class UBehaviorTree> _btAsset, TObjectPtr<class UBlackboardData> _bbAsset);

public:
	AMonsterAIController();
	
	virtual void OnPossess(APawn* InPawn) override;
};
