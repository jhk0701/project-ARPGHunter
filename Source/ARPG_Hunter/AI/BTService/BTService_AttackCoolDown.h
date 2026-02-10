// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_AttackCoolDown.generated.h"

/**
 * 별도의 동작과 병행하며 공격의 쿨타임을 기다리기 위해서 만든 서비스
 */
UCLASS()
class ARPG_HUNTER_API UBTService_AttackCoolDown : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_AttackCoolDown();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
