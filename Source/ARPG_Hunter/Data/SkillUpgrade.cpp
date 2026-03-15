// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUpgrade.h"

#include "Component/ActionComponent/PlayerActionComponent.h"
#include "Define/Enum.h"
#include "Data/EffectData.h"
#include "Action/ActionInstance.h"

void USkillNodeUnlockAction::AdjustSkillNode(FAdjustParam& _param)
{
	Super::AdjustSkillNode(_param);

	for (TPair<EAttackType, FActionConnect>& Start : *_param.GraphStart)
	{
		if (Start.Value.Index != TargetIndex)
			continue;

		Start.Value.bIsUnlocked = true;
	}

	for (TMap<EAttackType, FActionConnect>& Graph : *_param.Graph)
	{
		for (TPair<EAttackType, FActionConnect>& Edge : Graph)
		{
			if (Edge.Value.Index != TargetIndex)
				continue;

			Edge.Value.bIsUnlocked = true;
		}
	}
}

void USkillNodeModifyEffect::AdjustSkillNode(FAdjustParam& _param)
{
	Super::AdjustSkillNode(_param);

	(*_param.ActionArray)[TargetIndex]->AddEffectValue(TargetEffect->GetClass(), AddictiveValue);
}

void USkillNodeExtendEffect::AdjustSkillNode(FAdjustParam& _param)
{
	Super::AdjustSkillNode(_param);

	TObjectPtr<UActionInstance> ActionInst = (*_param.ActionArray)[TargetIndex];

	for (const TPair<EActionEvent, FActionEventEffect>& Pair : ExtendEffects)
	{
		for (TObjectPtr<UEffectData> EffectData : Pair.Value.Effects)
			ActionInst->ExtendEventEffect(Pair.Key, EffectData);
	}
}

void USkillNodeModifySpec::AdjustSkillNode(FAdjustParam& _param)
{
	Super::AdjustSkillNode(_param);

	TObjectPtr<UActionInstance> ActionInst = (*_param.ActionArray)[TargetIndex];
	ActionInst->AddAttackDamagePer(AddictiveAttackDamagePer);
	ActionInst->AddStaggerDamage(AddictiveStaggerDamage);
	ActionInst->AddReduceStaminaUsage(ReduceStaminaUsage);
}
