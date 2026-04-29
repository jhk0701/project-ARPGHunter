// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack.generated.h"

enum class EMonsterAttackType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Attack();

private:
	UPROPERTY(EditAnywhere)
	EMonsterAttackType AttackType;
	UPROPERTY(EditAnywhere)
	FName TargetValName;
	UPROPERTY(EditAnywhere)
	FName AttackIntervalValName;
	UPROPERTY(EditAnywhere)
	FName PickOffValName;

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnAttackEnded(UBehaviorTreeComponent& OwnerComp) {};
};

enum class EPlayerActionType : uint8;

UCLASS()
class ARPG_HUNTER_API UBTTask_ReactionAttack : public UBTTask_Attack
{
	GENERATED_BODY()

public:
	UBTTask_ReactionAttack();

private:
	UPROPERTY(EditAnywhere)
	FName TriggerVarName;

	UPROPERTY(EditAnywhere)
	EPlayerActionType ReactingPlayerAction;

protected:
	void OnAttackEnded(UBehaviorTreeComponent& OwnerComp) override;
};
