// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/Effect.h"
#include "Player/PlayerCharacter.h"
#include "Component/Stat/StatComponent.h"

void UEffect::Activate(uint16 _id, TWeakObjectPtr<AActor> _target, FEffectContext* _context)
{
	ID = _id;
	Target = _target;
	
	Value = _context->Value;
	Duration = _context->Duration;

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(_target))
		PlayerCharacter->AddEffect(this);
}
