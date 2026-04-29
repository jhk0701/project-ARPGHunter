// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_TargetIsInFront.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Define/Enum.h"

UBTDecorator_TargetIsInFront::UBTDecorator_TargetIsInFront()
{
	NodeName = TEXT("Target Is In Front");
	TargetValName = FName(TEXT("Target"));
	PickOffValName = FName(TEXT("PickOff"));
}

bool UBTDecorator_TargetIsInFront::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	TObjectPtr<APawn> Owner = OwnerComp.GetAIOwner()->GetPawn();
	TObjectPtr<UBlackboardComponent> BBComp = OwnerComp.GetBlackboardComponent();

	TObjectPtr<AActor> Target = Cast<AActor>(BBComp->GetValueAsObject(TargetValName));

	FVector Dist = Target->GetActorLocation() - Owner->GetActorLocation();
	Dist.Normalize();

	double Dot = FVector::DotProduct(Dist, Owner->GetActorForwardVector());
	double Degree = FMath::RadiansToDegrees(FMath::Acos(Dot));

	if (Degree < FrontDegreeRange) // 정면인 경우
	{
		BBComp->SetValueAsEnum(PickOffValName, static_cast<uint8>(EPickOff::NONE));
		return true;
	}

	EPickOff PickOffDir;

	if (Degree > 180.0f - BackDegreeRange) // 후면인 경우
		PickOffDir = EPickOff::BACKWARD;
	else
	{
		// 좌우 판정, 외적 결과, Z축이 음수 : 왼쪽, 양수 : 오른쪽
		FVector Crs = FVector::CrossProduct(Owner->GetActorForwardVector(), Dist);
		PickOffDir = Crs.Z > 0 ? EPickOff::RIGHTWARD : EPickOff::LEFTWARD;
	}

	BBComp->SetValueAsEnum(PickOffValName, static_cast<uint8>(PickOffDir));

	return false;
}
