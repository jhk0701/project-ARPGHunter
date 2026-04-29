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
	float FrontDegree{90.0f};
	UPROPERTY(EditAnywhere)
	FName TargetValName;

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
