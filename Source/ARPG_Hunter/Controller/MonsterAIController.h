// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();
	static constexpr uint8 MAX_MOVETO_RETRY_CNT = 5; 

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAIPerceptionComponent> AIPerception;
	uint8 MoveToRetryCnt{0};
	
	void PlayBT(APawn* _inPawn);

protected:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;

public:
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void StopBT();
	void RestartBT();

	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
};
