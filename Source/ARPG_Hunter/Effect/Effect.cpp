// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/Effect.h"

void UEffect::Activate(uint16 _id, TWeakObjectPtr<AActor> _target, FEffectContext* _context)
{
	ID = _id;
	Target = _target;

	Value = _context->Value;
	Duration = _context->Duration;
}
