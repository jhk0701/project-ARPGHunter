// Fill out your copyright notice in the Description page of Project Settings.


#include "Action/ActionInstance.h"

#include "Define/Enum.h"
#include "Data/Action.h"
#include "Data/EffectData.h"

void UActionInstance::SetAction(TObjectPtr<UAction> _action)
{
	Action = _action;
	
	for (const TPair<EActionEvent, FActionEventEffect>& Pair : Action->EventEffect)
	{
		FActionEventEffect& Array = EventEffect.Add(Pair.Key);
		Array.Effects = Pair.Value.Effects;
	}
}

uint16 UActionInstance::GetAttackDamagePer(uint8 _opt) const
{
	return Action->ArrOption[_opt].AttackDamagePer + AddictiveAttackDamagePer;
}

uint16 UActionInstance::GetStaggerDamage(uint8 _opt) const
{
	return Action->ArrOption[_opt].StaggerDamage + AddictiveStaggerDamage;
}

uint8 UActionInstance::GetStaminaUsage() const
{
	if (Action->StaminaUsage <= ReduceStaminaUsage)
		return 0;

	return Action->StaminaUsage - ReduceStaminaUsage;
}

void UActionInstance::AddEffectValue(UClass* _effectClass, uint32 _value)
{
	uint32& Value = AddictiveEffectValue.FindOrAdd(_effectClass);
	Value += _value;
}

uint32 UActionInstance::GetAddictiveEffectValue(TObjectPtr<UEffectData> _effect)
{
	if (AddictiveEffectValue.Contains(_effect->GetClass()))
		return AddictiveEffectValue[_effect->GetClass()];

	return 0;
}

void UActionInstance::ExtendEventEffect(EActionEvent _eventType, TObjectPtr<UEffectData> _effect)
{
	if (EventEffect.Contains(_eventType))
		EventEffect[_eventType].Effects.Add(_effect);
	else
		EventEffect.Add(_eventType, FActionEventEffect({ _effect }));
}
