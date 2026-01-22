// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_IsInAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Monster/MonsterBase.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("Is In Attack Range");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AMonsterBase* Owner = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (Owner == nullptr)
		return false;

	ACharacter* Target = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return false;

	float Range = Owner->GetAttackRange();
	float DistSqr = FVector::DistSquared(Owner->GetActorLocation(), Target->GetActorLocation());
	
	return DistSqr < FMath::Square(Range);
}
