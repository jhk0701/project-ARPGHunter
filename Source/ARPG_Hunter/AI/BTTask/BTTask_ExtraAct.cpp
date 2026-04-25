// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_ExtraAct.h"
#include "AIController.h"

#include "Monster/MonsterBase.h"

UBTTask_ExtraAct::UBTTask_ExtraAct()
{
	NodeName = TEXT("Extra Act");
	ActName = NAME_None;
}

EBTNodeResult::Type UBTTask_ExtraAct::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ActName == NAME_None)
		return EBTNodeResult::Failed;

	AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Monster)
		return EBTNodeResult::Failed;

	if (false == Monster->ExtraAct(ActName))
		return EBTNodeResult::Failed;

	Monster->OnExtraActMontageEnded.BindLambda(
		[this, &OwnerComp]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	return EBTNodeResult::InProgress;
}
