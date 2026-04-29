// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_TargetIsInFront.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_TargetIsInFront::UBTDecorator_TargetIsInFront()
{
	NodeName = TEXT("Target Is In Front");
	TargetValName = FName(TEXT("Target"));
}

bool UBTDecorator_TargetIsInFront::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetValName));

	FVector Dist = Target->GetActorLocation() - Owner->GetActorLocation();
	Dist.Normalize();

	double Degree = FMath::RadiansToDegrees(FVector::DotProduct(Dist, Owner->GetActorForwardVector()));
	return Degree < FrontDegree;
}
