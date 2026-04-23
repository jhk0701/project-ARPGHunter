// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_TransitionAlertState.h"
#include "BTTask_TransitionToAlert.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTTask_TransitionToAlert : public UBTTask_TransitionAlertState
{
	GENERATED_BODY()

public:
	UBTTask_TransitionToAlert();

private:
	UPROPERTY(EditAnywhere)
	FName ElapsedTimeName;

	UPROPERTY(EditAnywhere)
	float ElapsedTimeSet{ 10.0f };

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
