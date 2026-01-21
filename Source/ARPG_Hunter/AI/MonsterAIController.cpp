// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MonsterAIController.h"
#include "Monster/MonsterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBT(InPawn);
}

void AMonsterAIController::PlayBT(APawn* _inPawn)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(_inPawn);
	if (Monster == nullptr)
		return;

	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (BlackboardComp == nullptr)
		return;

	if (UseBlackboard(Monster->GetBlackboardData(), BlackboardComp))
		ensure(RunBehaviorTree(Monster->GetBehaviorTree()));
}
