// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_TargetIsValid.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Player/PlayerCharacter.h"


UBTDecorator_TargetIsValid::UBTDecorator_TargetIsValid()
{
	NodeName = TEXT("Target Is Valid");

	TargetVarName = FName(TEXT("Target"));
}

bool UBTDecorator_TargetIsValid::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
 	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetVarName);

	if (nullptr == Target)
		return false;

	APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(Target);
	if (nullptr == PlayerChar)
		return false;

	return false == PlayerChar->IsDead();
}
