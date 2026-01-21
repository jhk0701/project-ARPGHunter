// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MonsterAIController.h"
#include "Monster/MonsterBase.h"

AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	
	// PlayBT();
}

void AMonsterAIController::PlayBT(TObjectPtr<class UBehaviorTree> _btAsset, TObjectPtr<class UBlackboardData> _bbAsset)
{
}
