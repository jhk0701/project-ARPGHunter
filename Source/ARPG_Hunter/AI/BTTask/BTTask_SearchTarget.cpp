// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SearchTarget.h"


UBTTask_SearchTarget::UBTTask_SearchTarget()
{
	NodeName = TEXT("Search Target");
}

EBTNodeResult::Type UBTTask_SearchTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("[UBTTask_SearchTarget] Search Target"));
	// OwnerComp.GetAIOwner

	return EBTNodeResult::Succeeded;
}
