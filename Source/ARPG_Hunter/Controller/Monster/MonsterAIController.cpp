// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/Monster/MonsterAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AISenseConfig_Team.h"
#include "AI/Sense/AISense_PlayerAction.h"

#include "Define/Enum.h"
#include "Monster/MonsterBase.h"

AMonsterAIController::AMonsterAIController()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	AIStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComp"));

	AlertStateName = FName(TEXT("AlertState"));
	ActorGroup = EActorGroup::HOSTILE;
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ControlledMonster = Cast<AMonsterBase>(InPawn);

	SetGenericTeamId(FGenericTeamId(static_cast<uint8>(ActorGroup)));
	if (UAIPerceptionSystem* AIPerceptSys = UAIPerceptionSystem::GetCurrent(GetWorld()))
		AIPerceptSys->UpdateListener(*AIPerception);

	InitBT(InPawn);
	StopPerception();
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

	AIStimuliSource->UnregisterFromPerceptionSystem();
}

void AMonsterAIController::RestartPerception()
{
	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AMonsterAIController::OnTargetPerceptionUpdated);
	AIPerception->Activate();
	AIPerception->RequestStimuliListenerUpdate();

	AIStimuliSource->RegisterWithPerceptionSystem();
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
		if (Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
			HandleSuspicious(Actor, Stimulus);
		else if (Stimulus.Type == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
			HandleDamage(Actor, Stimulus);
		else if (Stimulus.Type == UAISense::GetSenseID(UAISense_Team::StaticClass()))
			HandleTeamDamage(Actor, Stimulus);
		else if (Stimulus.Type == UAISense::GetSenseID(UAISense_PlayerAction::StaticClass()))
			HandlePlayerAction(Actor, Stimulus);
	}
	else
		MissTarget(Actor);
}

void AMonsterAIController::HandleSuspicious(AActor* _actor, struct FAIStimulus& _stimulus)
{
	uint8 CurAlert = GetAlertState();
	if (CurAlert >= static_cast<uint8>(EMonsterAlertState::ENAGE))
		return;

	UBlackboardComponent* BBComp = GetBlackboardComponent();
	BBComp->SetValueAsEnum(AlertStateName, static_cast<uint8>(EMonsterAlertState::SUSPICIOUS));
	BBComp->SetValueAsVector(FName(TEXT("MovePoint")), _stimulus.StimulusLocation);
	BBComp->SetValueAsObject(FName(TEXT("Target")), _actor);
}

void AMonsterAIController::HandleDamage(AActor* _actor, struct FAIStimulus& _stimulus)
{
	// 처음 피격 시에만 작동
	uint8 CurAlert = GetAlertState();
	if (CurAlert >= static_cast<uint8>(EMonsterAlertState::ENAGE))
		return;

	SetEnageState(_actor);

	// 피격 시, 주변 몬스터들에게 피격 이벤트 발행
	FAITeamStimulusEvent TeamEvent = FAITeamStimulusEvent(
		GetPawn(),
		_actor, 
		_stimulus.StimulusLocation, 
		TeamSenseRange);
	TeamEvent.TeamIdentifier = GetGenericTeamId();

	UAIPerceptionSystem::OnEvent<FAITeamStimulusEvent, FAITeamStimulusEvent::FSenseClass>(GetWorld(), TeamEvent);
}

void AMonsterAIController::HandleTeamDamage(AActor* _actor, FAIStimulus& _stimulus)
{
	uint8 CurAlert = GetAlertState();
	if (CurAlert >= static_cast<uint8>(EMonsterAlertState::ENAGE))
		return;

	SetEnageState(_actor);
}

void AMonsterAIController::HandlePlayerAction(AActor* _actor, FAIStimulus& _stimulus)
{
	uint8 CurAlert = GetAlertState();
	if (CurAlert != static_cast<uint8>(EMonsterAlertState::ENAGE))
		return;

	if (AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn()))
		Monster->TriggerReactForPlayerAction(static_cast<uint8>(_stimulus.Strength));
}

void AMonsterAIController::MissTarget(AActor* _actor)
{
	uint8 CurAlert = GetAlertState();
	if (CurAlert >= static_cast<uint8>(EMonsterAlertState::ALERT))
		return;

	GetBlackboardComponent()->ClearValue(FName(TEXT("Target")));
}

void AMonsterAIController::SetEnageState(AActor* _target)
{
	UBlackboardComponent* BBComp = GetBlackboardComponent();

	if (AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn()))
		Monster->OnAlertStateChanged(static_cast<EMonsterAlertState>(GetAlertState()), EMonsterAlertState::ENAGE);

	BBComp->SetValueAsEnum(AlertStateName, static_cast<uint8>(EMonsterAlertState::ENAGE));
	BBComp->SetValueAsObject(FName(TEXT("Target")), _target);
}

uint8 AMonsterAIController::GetAlertState()
{
	return GetBlackboardComponent()->GetValueAsEnum(AlertStateName);
}
