// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_CanDoRangedAttack.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTDecorator_CanDoRangedAttack::UBTDecorator_CanDoRangedAttack()
{
	NodeName = TEXT("Check Can Do Ranged Attack");
}

bool UBTDecorator_CanDoRangedAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{


	return false;
}
