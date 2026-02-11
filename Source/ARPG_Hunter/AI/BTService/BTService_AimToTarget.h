// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AimToTarget.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTService_AimToTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AimToTarget();

private:
	UPROPERTY(EditAnywhere)
	float AimRange{ 60.0f };
	UPROPERTY(EditAnywhere)
	float RotateSpeed{ 10.0 };
	UPROPERTY(EditAnywhere)
	bool bCheckMovable{ true };

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
