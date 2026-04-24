// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_CheckOutOfRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_CheckOutOfRange::UBTDecorator_CheckOutOfRange()
{
	NodeName = TEXT("Check Out Of Range");

	AnchorVarName = FName(TEXT("ReturnPoint"));
	RangeVarName = FName(TEXT("ActivityRange"));
}

bool UBTDecorator_CheckOutOfRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	
	FVector ReturnPoint = BBComp->GetValueAsVector(AnchorVarName);
	float Range = BBComp->GetValueAsFloat(RangeVarName);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	FVector DistVec = Owner->GetActorLocation() - ReturnPoint;
	
	return DistVec.SquaredLength() > Range * Range;
}
