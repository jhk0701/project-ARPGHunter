// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_TransitionToAlert.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Define/Enum.h"

UBTTask_TransitionToAlert::UBTTask_TransitionToAlert()
{
	NodeName = TEXT("ToAlertState");

	ElapsedTimeName = FName(TEXT("AlertElapsedTime"));
	TargetState = EMonsterAlertState::ALERT;
}

EBTNodeResult::Type UBTTask_TransitionToAlert::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ElapsedTimeName, ElapsedTimeSet);
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}
