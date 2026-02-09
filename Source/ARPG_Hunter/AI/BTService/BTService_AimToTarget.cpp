// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService/BTService_AimToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"


UBTService_AimToTarget::UBTService_AimToTarget()
{
	NodeName = TEXT("Aim To Target");
	Interval = 0.1f;
	bTickIntervals = true;
}

void UBTService_AimToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UObject* Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target")));
	if (nullptr == Target)
		return;

	TObjectPtr<AActor> TargetActor = Cast<AActor>(Target);
	TObjectPtr<AActor> OwnerActor = OwnerComp.GetAIOwner()->GetPawn();

	FVector OwnerFwd = OwnerActor->GetActorForwardVector();
	FVector DirToTarget = TargetActor->GetActorLocation() - OwnerActor->GetActorLocation();
	DirToTarget.Z = 0.0f;
	DirToTarget.Normalize();
	
	// 1. 시야 범위를 벗어났는지 체크
	double dot = FVector::DotProduct(DirToTarget, OwnerFwd);
	if (dot > 0 && FMath::RadiansToDegrees(FMath::Acos(dot)) < AimRange)
		return; // 시야 범위 내

	// 2. 돌리기
	FRotator Rot(0.0f, FMath::RadiansToDegrees(FMath::Atan2(DirToTarget.Y, DirToTarget.X)) * RotateSpeed *  DeltaSeconds, 0.0f);
	OwnerActor->SetActorRotation(Rot.Quaternion());
}
