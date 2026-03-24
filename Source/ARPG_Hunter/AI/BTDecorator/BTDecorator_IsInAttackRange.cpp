// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_IsInAttackRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Monster/MonsterBase.h"
#include "Player/PlayerCharacter.h"

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

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	APlayerCharacter* Target = Cast<APlayerCharacter>(BBComp->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr || Target->IsDead())
		return false;

	float Range = BBComp->GetValueAsFloat(FName(TEXT("AttackRange")));
	float DistSqr = FVector::DistSquared(Owner->GetActorLocation(), Target->GetActorLocation());
	
	return DistSqr <= FMath::Square(Range);
}
