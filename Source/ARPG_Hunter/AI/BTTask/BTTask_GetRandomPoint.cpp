// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_GetRandomPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

#include "Monster/MonsterBase.h"

UBTTask_GetRandomPoint::UBTTask_GetRandomPoint()
{
}

EBTNodeResult::Type UBTTask_GetRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterBase* Owner = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(Owner);
	if (NavSys == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation RandomLoc;
	float RecogRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(FName(TEXT("RecoginitionRange")));
	if (NavSys->GetRandomPointInNavigableRadius(Owner->GetActorLocation(), RecogRange, RandomLoc) == false)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (BlackBoardComp == nullptr)
		return EBTNodeResult::Failed;

	BlackBoardComp->SetValueAsVector(FName(TEXT("MovePoint")), RandomLoc.Location);

	return EBTNodeResult::Succeeded;
}
