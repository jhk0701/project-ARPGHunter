// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AlertSearching.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTService_AlertSearching : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AlertSearching();

private:
	UPROPERTY(EditAnywhere)
	FName ElapsedTimeName;
	UPROPERTY(EditAnywhere)
	float LimitTime{ 10.0f };

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
