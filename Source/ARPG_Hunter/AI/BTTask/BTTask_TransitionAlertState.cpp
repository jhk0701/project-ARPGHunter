// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_TransitionAlertState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Define/Enum.h"
#include "Monster/MonsterBase.h"

UBTTask_TransitionAlertState::UBTTask_TransitionAlertState()
{
	NodeName = TEXT("Transition Alert State");

	VarName = FName(TEXT("AlertState"));
}

EBTNodeResult::Type UBTTask_TransitionAlertState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

	uint8 CurAlertState = BBComp->GetValueAsEnum(VarName);

	if (AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn()))
		Monster->OnAlertStateChanged(static_cast<EMonsterAlertState>(CurAlertState), TargetState);

	BBComp->SetValueAsEnum(VarName, static_cast<uint8>(TargetState));

	return EBTNodeResult::Succeeded;
}
