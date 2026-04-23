// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_TransitionAlertState.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Define/Enum.h"


UBTTask_TransitionAlertState::UBTTask_TransitionAlertState()
{
	NodeName = TEXT("Transition Alert State");

	VarName = FName(TEXT("AlertState"));
}

EBTNodeResult::Type UBTTask_TransitionAlertState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	OwnerComp.GetBlackboardComponent()->SetValueAsEnum(VarName, static_cast<uint8>(TargetState));

	return EBTNodeResult::Succeeded;
}
