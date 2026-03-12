// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MonsterAIController.h"
#include "Monster/MonsterBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Navigation/PathFollowingComponent.h"


AMonsterAIController::AMonsterAIController()
{
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayBT(InPawn);
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

void AMonsterAIController::PlayBT(APawn* _inPawn)
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