// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TransitionAlertState.generated.h"

enum class EMonsterAlertState : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTTask_TransitionAlertState : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_TransitionAlertState();

private:
	UPROPERTY(EditAnywhere)
	FName VarName;

protected:
	UPROPERTY(EditAnywhere)
	EMonsterAlertState TargetState;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
