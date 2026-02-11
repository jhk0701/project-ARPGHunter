// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_CanUseSkill.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controller/MonsterAIController.h"

#include "Monster/BossMonster/BossMonster.h"


UBTDecorator_CanUseSkill::UBTDecorator_CanUseSkill()
{
	NodeName = TEXT("Can Use Skill");
}

bool UBTDecorator_CanUseSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	TObjectPtr<ABossMonster> Monster = Cast<ABossMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
		return false;
	
	return Monster->CanUseSkill();
}
