// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Define/Enum.h"
#include "Monster/MonsterBase.h"
#include "Player/PlayerCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterBase* Owner = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Owner == nullptr)
		return EBTNodeResult::Failed;

	APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target")))); 
	if (Target == nullptr)
		return EBTNodeResult::Failed;
	
	Owner->OnAttackMontageEnded.BindLambda(
		[&]()
		{
			APlayerCharacter* Target = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

			if (Target == nullptr || Target->IsDead())
				OwnerComp.GetBlackboardComponent()->ClearValue(FName(TEXT("Target")));

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Attack Montage is Ended And Attack Task Succeeded"));
		}
	);

	FMonsterAttackParam Param;
	Param.Type = AttackType;
	Owner->Attack(&Param);

	return EBTNodeResult::InProgress;
}
