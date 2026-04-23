// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Chase.h"
#include "AIController.h"

#include "Monster/MonsterBase.h"

UBTTask_Chase::UBTTask_Chase()
{
	NodeName = TEXT("Set Chase");
}

EBTNodeResult::Type UBTTask_Chase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Monster->SetMoveSpeed(bIsOn);
	}

	return EBTNodeResult::Succeeded;
}
