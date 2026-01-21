// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTTask_GetRandomPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_GetRandomPoint();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
