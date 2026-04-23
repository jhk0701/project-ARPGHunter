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
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetPerceptionUpdated);
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
	if (MoveToRetryCnt >= MaxMoveToRetryCnt)
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

	AIPerception->OnTargetPerceptionUpdated.RemoveDynamic(this, &AMonsterAIController::OnTargetPerceptionUpdated);
}

void AMonsterAIController::RestartPerception()
{
	
	AIPerception->Activate();
	AIPerception->RequestStimuliListenerUpdate();

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetPerceptionUpdated);
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

void AMonsterAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("[%s] is Percepted"), *Actor->GetActorNameOrLabel()));

		if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()) ||
			Stimulus.Type == UAISense::GetSenseID(UAISense_Hearing::StaticClass()))
			HandleSuspicious(Actor, Stimulus);
		else if (Stimulus.Type == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
			HandleDamage(Actor, Stimulus);
		else if (Stimulus.Type == UAISense::GetSenseID(UAISense_Team::StaticClass()))
			HandleTeamDamage(Actor, Stimulus);
	}
	else
	{
		MissTarget(Actor);
	}
}

void AMonsterAIController::HandleSuspicious(AActor* _actor, struct FAIStimulus& _stimulus)
{
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	const FName NAME_ALERTSTATE = FName(TEXT("AlertState"));

	uint8 CurAlert = BBComp->GetValueAsEnum(NAME_ALERTSTATE);
	if (CurAlert >= static_cast<uint8>(EMonsterAlertState::ENAGE))
		return;

	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("AI Suspicious"));

	BBComp->SetValueAsEnum(NAME_ALERTSTATE, static_cast<uint8>(EMonsterAlertState::SUSPICIOUS));
	BBComp->SetValueAsVector(FName(TEXT("MovePoint")), _stimulus.StimulusLocation);
	BBComp->SetValueAsObject(FName(TEXT("Target")), _actor);
}

void AMonsterAIController::HandleDamage(AActor* _actor, struct FAIStimulus& _stimulus)
{
	// GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("AI Damaged"));

	UBlackboardComponent* BBComp = GetBlackboardComponent();
	const FName NAME_ALERTSTATE = FName(TEXT("AlertState"));
	uint8 CurAlert = BBComp->GetValueAsEnum(NAME_ALERTSTATE);

	if (CurAlert >= static_cast<uint8>(EMonsterAlertState::ENAGE))
		return;

	BBComp->SetValueAsEnum(NAME_ALERTSTATE, static_cast<uint8>(EMonsterAlertState::ENAGE));
	BBComp->SetValueAsObject(FName(TEXT("Target")), _actor);
}

void AMonsterAIController::HandleTeamDamage(AActor* _actor, FAIStimulus& _stimulus)
{
}

void AMonsterAIController::MissTarget(AActor* _actor)
{
	UBlackboardComponent* BBComp = GetBlackboardComponent();
	const FName NAME_ALERTSTATE = FName(TEXT("AlertState"));

	uint8 CurAlert = BBComp->GetValueAsEnum(NAME_ALERTSTATE);
	if (CurAlert >= static_cast<uint8>(EMonsterAlertState::ALERT))
		return;

	BBComp->ClearValue(FName(TEXT("Target")));

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("[%s] is Missed"), *_actor->GetActorNameOrLabel()));
}

void AMonsterAIController::ReleaseAlert()
{
}
