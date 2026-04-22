// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Team.h"

#include "Monster/MonsterBase.h"
#include "Define/Enum.h"

AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
}

void AMonsterAIController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnPerceptionUpdated);
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledMonster = Cast<AMonsterBase>(InPawn);

	InitBT(InPawn);
	StopPerception();
}

void AMonsterAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	// 원하는 곳으로 못갈 때 처리
	// 주로, 플레이어가 네비 메시를 벗어나 갈 수 없을 경우 처리용
	MoveToRetryCnt = Result.IsSuccess() ? 0 : MoveToRetryCnt + 1;
	if (MoveToRetryCnt >= MAX_MOVETO_RETRY_CNT)
	{
		MoveToRetryCnt = 0;
		// 타겟 초기화
		GetBlackboardComponent()->ClearValue(FName(TEXT("Target")));

		if (AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn()))
			Monster->OnTargetLost();
	}
}

void AMonsterAIController::InitBT(APawn* _inPawn)
{
	AMonsterBase* Monster = Cast<AMonsterBase>(_inPawn);
	if (Monster == nullptr)
		return;

	UBlackboardComponent* BlackboardComp = Blackboard.Get();
	if (UseBlackboard(Monster->GetBlackboardData(), BlackboardComp))
	{
		RunBehaviorTree(Monster->GetBehaviorTree());
		StopBT();
	}
}

void AMonsterAIController::StopBT()
{
	GetBlackboardComponent()->ClearValue(FName(TEXT("Target")));

	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	BTComp->StopTree();
}

void AMonsterAIController::RestartBT()
{
	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	BTComp->RestartTree();
}

void AMonsterAIController::StopPerception()
{
	AIPerception->ForgetAll();
	AIPerception->Deactivate();
}

void AMonsterAIController::RestartPerception()
{
	AIPerception->Activate();
	AIPerception->RequestStimuliListenerUpdate();
}

void AMonsterAIController::EnableController()
{
	RestartBT();
	RestartPerception();
}

void AMonsterAIController::DisableController()
{
	StopBT();
	StopPerception();
}

void AMonsterAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("[%s] Perception updated"), *GetPawn()->GetActorNameOrLabel()));
	for (AActor* Actor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		if (false == AIPerception->GetActorsPerception(Actor, Info))
			continue;

		for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
		{
			if (Stimulus.IsExpired() || 
				false == Stimulus.WasSuccessfullySensed()) 
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("[%s] Not Valid Stimulus"), *GetPawn()->GetActorNameOrLabel()));
				continue;
			}
			
			UBlackboardComponent* BBComp = GetBlackboardComponent();

			if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>() ||
				Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>())
			{
				// 상태 전환 : 주의-경계
				HandleSuspicious(Stimulus.StimulusLocation);
				return;
			}
			else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>() || 
				Stimulus.Type == UAISense::GetSenseID<UAISense_Team>())
			{
				// 상태 전환 : 전투
				HandleEngage();
				return;
			}
		}
	}
}

void AMonsterAIController::HandleSuspicious(const FVector& _location)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("AI Suspicious"));
}

void AMonsterAIController::HandleEngage()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("AI Engage"));
}
