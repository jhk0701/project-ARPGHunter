// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_AttackCoolDown.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_AttackCoolDown::UBTService_AttackCoolDown()
{
	NodeName = TEXT("Attack CoolDown");
	bTickIntervals = true;
	Interval = 0.5f;
}

void UBTService_AttackCoolDown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	FName ValName = FName(TEXT("AttackInterval"));

	float RemainInterval = BBComp->GetValueAsFloat(ValName);

	if (RemainInterval > 0.0f)
		RemainInterval -= DeltaSeconds;

	BBComp->SetValueAsFloat(ValName, RemainInterval);
}
