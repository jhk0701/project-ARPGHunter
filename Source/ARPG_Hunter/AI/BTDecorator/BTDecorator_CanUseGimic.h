// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanUseGimic.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTDecorator_CanUseGimic : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CanUseGimic();

protected:
	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
