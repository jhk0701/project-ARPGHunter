// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

enum class EMonsterAlertState : uint8;
enum class EActorGroup : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMonsterAIController();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UAIPerceptionComponent> AIPerception;
	UPROPERTY(EditAnywhere) // 팀 피격 이벤트 전파 용도
	TObjectPtr<class UAIPerceptionStimuliSourceComponent> AIStimuliSource;
	TObjectPtr<class AMonsterBase> ControlledMonster;

	UPROPERTY(EditAnywhere)
	FName AlertStateName;
	UPROPERTY(EditAnywhere)
	float TeamSenseRange{ 1000.0f };

	UPROPERTY(EditAnywhere)
	EActorGroup ActorGroup;

	void InitBT(APawn* _inPawn);

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void StopBT();
	void RestartBT();

	void StopPerception();
	void RestartPerception();

	void EnableController();
	void DisableController();

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus);

	void HandleSuspicious(AActor* _actor, struct FAIStimulus& _stimulus);
	void HandleDamage(AActor* _actor, struct FAIStimulus& _stimulus);
	void HandleTeamDamage(AActor* _actor, struct FAIStimulus& _stimulus);
	void HandlePlayerAction(AActor* _actor, struct FAIStimulus& _stimulus);
	void MissTarget(AActor* _actor);

	void SetEnageState(AActor* _target);
	uint8 GetAlertState();
};
