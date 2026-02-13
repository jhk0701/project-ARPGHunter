// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_CanUseGimic.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Controller/MonsterAIController.h"
#include "Interface/GimicHandler.h"


UBTDecorator_CanUseGimic::UBTDecorator_CanUseGimic()
{
	NodeName = TEXT("Can Use Gimic");
}

bool UBTDecorator_CanUseGimic::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	IGimicHandler* GimicHandler = Cast<IGimicHandler>(OwnerComp.GetAIOwner()->GetPawn());
	if (GimicHandler == nullptr)
		return false;
	
	return GimicHandler->CanUseGimic();
}
