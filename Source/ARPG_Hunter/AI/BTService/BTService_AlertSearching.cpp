// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_AlertSearching.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AlertSearching::UBTService_AlertSearching()
{
	NodeName = TEXT("Alert Searching");

	bTickIntervals = true;
	Interval = 1.0f;

	ElapsedTimeName = FName(TEXT("AlertElapsedTime"));
}

void UBTService_AlertSearching::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
 	float CurElapsedTime = BBComp->GetValueAsFloat(ElapsedTimeName);
	CurElapsedTime -= DeltaSeconds;

	BBComp->SetValueAsFloat(ElapsedTimeName, CurElapsedTime);
}
