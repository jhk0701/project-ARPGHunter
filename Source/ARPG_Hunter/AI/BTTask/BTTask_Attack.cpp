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
	TargetValName = FName(TEXT("Target"));
	AttackIntervalValName = FName(TEXT("AttackInterval"));
	PickOffValName = FName(TEXT("PickOff"));
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	TObjectPtr<AMonsterBase> Owner = Cast<AMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(Owner == nullptr)
		return EBTNodeResult::Failed;

	TObjectPtr<UBlackboardComponent> BBComp = OwnerComp.GetBlackboardComponent();
	TObjectPtr<APlayerCharacter> Target = Cast<APlayerCharacter>(BBComp->GetValueAsObject(TargetValName));
	if (Target == nullptr)
		return EBTNodeResult::Failed;
	
	Owner->OnAttackMontageEnded.BindLambda(
		[this, &OwnerComp]()
		{
			TObjectPtr<UBlackboardComponent> BBComp = OwnerComp.GetBlackboardComponent();
			TObjectPtr<APlayerCharacter> Target = Cast<APlayerCharacter>(BBComp->GetValueAsObject(TargetValName));

			if (Target == nullptr || Target->IsDead())
				BBComp->ClearValue(TargetValName);

			OnAttackEnded(OwnerComp);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	EPickOff CurPickOff = static_cast<EPickOff>(BBComp->GetValueAsEnum(PickOffValName));
	float Interval = Owner->Attack(AttackType, EnumToName(CurPickOff));

	if (Interval < 0.0f)
		return EBTNodeResult::Failed; // 공격 동작이 유효하지 않은 상황 실패처리

	BBComp->SetValueAsFloat(AttackIntervalValName, Interval); // 공격 후 대기시간

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
