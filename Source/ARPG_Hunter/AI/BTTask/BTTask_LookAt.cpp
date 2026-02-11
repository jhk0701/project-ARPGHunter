// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_LookAt.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_LookAt::UBTTask_LookAt()
{
	NodeName = TEXT("Loot At");
}

EBTNodeResult::Type UBTTask_LookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AActor> OwnerActor = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target")));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	TObjectPtr<AActor> TargetActor = Cast<AActor>(Target);

	FVector DirToTarget = TargetActor->GetActorLocation() - OwnerActor->GetActorLocation();
	DirToTarget.Z = 0.0f;
	DirToTarget.Normalize();

	// 2. 돌리기
	FRotator TargetRot = OwnerActor->GetActorRotation();
	TargetRot.Yaw = FMath::RadiansToDegrees(FMath::Atan2(DirToTarget.Y, DirToTarget.X));
	OwnerActor->SetActorRotation(TargetRot);

	return EBTNodeResult::Succeeded;
}

