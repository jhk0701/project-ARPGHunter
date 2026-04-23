// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SearchTarget.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Define/Enum.h"

UBTTask_SearchTarget::UBTTask_SearchTarget()
{
	NodeName = TEXT("Search Target");
}

EBTNodeResult::Type UBTTask_SearchTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	UObject* Target = BBComp->GetValueAsObject(FName(TEXT("Target")));

	if (Target)
		BBComp->SetValueAsEnum(FName(TEXT("AlertState")), static_cast<uint8>(EMonsterAlertState::ENAGE));

	return EBTNodeResult::Succeeded;
}
