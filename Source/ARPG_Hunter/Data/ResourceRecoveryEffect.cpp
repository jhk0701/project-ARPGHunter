// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/ResourceRecoveryEffect.h"
#include "Player/PlayerCharacter.h"
#include "Component/Stat/PlayerStatComponent.h"

void UResourceRecoveryEffect::ActivateEffect(const FEffectContext& _context)
{
	if (APlayerCharacter* TargetChar = Cast<APlayerCharacter>(_context.Target))
	{
		switch (TargetType)
		{
		case EResourceType::HEALTH:
			TargetChar->GetStatComp()->RecoverHealth(_context.EffectValue);
			break;
		case EResourceType::STAMINA:
			TargetChar->GetStatComp()->RecoverStamina(_context.EffectValue);
			break;
		case EResourceType::SKILL:
			TargetChar->GetStatComp()->RecoverSkill(_context.EffectValue);
			break;
		}
	}
}
