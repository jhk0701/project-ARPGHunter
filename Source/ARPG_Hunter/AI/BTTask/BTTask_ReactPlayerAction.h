// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_ExtraAct.h"
#include "BTTask_ReactPlayerAction.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTTask_ReactPlayerAction : public UBTTask_ExtraAct
{
	GENERATED_BODY()

public:
	UBTTask_ReactPlayerAction();

private:
	UPROPERTY(EditAnywhere)
	FName TriggerVarName;
	UPROPERTY(EditAnywhere)
	FName ProbabilityVarName;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
