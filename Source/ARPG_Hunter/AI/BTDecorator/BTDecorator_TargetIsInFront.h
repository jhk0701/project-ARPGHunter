// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_TargetIsInFront.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTDecorator_TargetIsInFront : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_TargetIsInFront();

private:
	UPROPERTY(EditAnywhere)
	FName TargetValName;
	UPROPERTY(EditAnywhere)
	FName PickOffValName;

	UPROPERTY(EditAnywhere)
	float FrontDegreeRange{ 60.0f };
	UPROPERTY(EditAnywhere)
	float BackDegreeRange{ 30.0f };

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
