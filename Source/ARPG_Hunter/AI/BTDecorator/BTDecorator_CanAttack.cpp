// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_CanAttack.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"
#include "Monster/MonsterBase.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("Check Can Do Attack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	TObjectPtr<AMonsterBase> Owner = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	return Owner->IsAttackable();
}
