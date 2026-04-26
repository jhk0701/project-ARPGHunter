// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_ReactPlayerAction.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ReactPlayerAction::UBTTask_ReactPlayerAction()
{
	NodeName = TEXT("React Player Action");

	TriggerVarName = FName(TEXT("bPlayerActionTrigger"));
	ProbabilityVarName = FName(TEXT("PlayerActionReactProbability"));
}

EBTNodeResult::Type UBTTask_ReactPlayerAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	BBComp->SetValueAsBool(TriggerVarName, false);

	uint8 r = FMath::Rand() % 100;
	if (r > BBComp->GetValueAsFloat(ProbabilityVarName))
		return EBTNodeResult::Succeeded;

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
