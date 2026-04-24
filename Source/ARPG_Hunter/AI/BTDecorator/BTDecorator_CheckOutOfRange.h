// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckOutOfRange.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTDecorator_CheckOutOfRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckOutOfRange();

private:
	UPROPERTY(EditAnywhere)
	FName AnchorVarName;
	UPROPERTY(EditAnywhere)
	FName RangeVarName;

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
