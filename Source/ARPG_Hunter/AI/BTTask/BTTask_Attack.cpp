// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_Attack.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Define/Enum.h"
#include "Monster/MonsterBase.h"
#include "Player/PlayerCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonsterBase* Owner = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Owner == nullptr)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	APlayerCharacter* Target = Cast<APlayerCharacter>(BBComp->GetValueAsObject(FName(TEXT("Target"))));
	if (Target == nullptr)
		return EBTNodeResult::Failed;
	
	Owner->OnAttackMontageEnded.BindLambda(
		[this, &OwnerComp]()
		{
			UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
			APlayerCharacter* Target = Cast<APlayerCharacter>(BBComp->GetValueAsObject(FName(TEXT("Target"))));

			if (Target == nullptr || Target->IsDead())
				BBComp->ClearValue(FName(TEXT("Target")));

			OnAttackEnded(OwnerComp);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	float Interval = Owner->Attack(AttackType, Target);

	if (Interval < 0.0f)
		return EBTNodeResult::Failed; // 공격 동작이 유효하지 않은 상황 실패처리

	BBComp->SetValueAsFloat(FName(TEXT("AttackInterval")), Interval); // 공격 후 대기시간

	return EBTNodeResult::InProgress;
}

UBTTask_ReactionAttack::UBTTask_ReactionAttack()
{
	NodeName = TEXT("Reaction Attack");
	TriggerVarName = FName(TEXT("bPlayerActionTrigger"));
}

void UBTTask_ReactionAttack::OnAttackEnded(UBehaviorTreeComponent& OwnerComp)
{
	// 처리후 정리
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TriggerVarName, false);

	if (AMonsterBase* Monster = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn()))
		Monster->SetReactToPlayerAction(ReactingPlayerAction, false);
}
