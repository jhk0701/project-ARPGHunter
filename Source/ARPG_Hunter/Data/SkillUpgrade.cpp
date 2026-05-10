// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillUpgrade.h"

#include "Component/Action/Player/PlayerActionComponent.h"
#include "Define/Enum.h"
#include "Data/EffectData.h"
#include "Action/ActionInstance.h"

void USkillNodeUnlockAction::AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param)
{
	Super::AdjustSkillNode(_targetIdx, _param);

	for (TPair<EAttackType, FActionConnect>& Start : *_param.GraphStart)
	{
		if (Start.Value.Index != _targetIdx)
			continue;

		Start.Value.bIsUnlocked = true;
	}

	for (TMap<EAttackType, FActionConnect>& Graph : *_param.Graph)
	{
		for (TPair<EAttackType, FActionConnect>& Edge : Graph)
		{
			if (Edge.Value.Index != _targetIdx)
				continue;

			Edge.Value.bIsUnlocked = true;
		}
	}
}

void USkillNodeModifyEffect::AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param)
{
	Super::AdjustSkillNode(_targetIdx, _param);

	(*_param.ActionArray)[_targetIdx]->AddEffectValue(TargetEffect->GetClass(), AddictiveValue);
}

void USkillNodeExtendEffect::AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param)
{
	Super::AdjustSkillNode(_targetIdx, _param);

	TObjectPtr<UActionInstance> ActionInst = (*_param.ActionArray)[_targetIdx];

	for (const TPair<EActionEvent, FActionEventEffect>& Pair : ExtendEffects)
	{
		for (TObjectPtr<UEffectData> EffectData : Pair.Value.Effects)
			ActionInst->ExtendEventEffect(Pair.Key, EffectData);
	}
}

void USkillNodeModifySpec::AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param)
{
	Super::AdjustSkillNode(_targetIdx, _param);

	TObjectPtr<UActionInstance> ActionInst = (*_param.ActionArray)[_targetIdx];
	ActionInst->AddAttackDamagePer(AddictiveAttackDamagePer);
	ActionInst->AddStaggerDamage(AddictiveStaggerDamage);
	ActionInst->AddReduceStaminaUsage(ReduceStaminaUsage);
}
