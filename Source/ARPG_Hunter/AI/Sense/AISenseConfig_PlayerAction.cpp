// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Sense/AISenseConfig_PlayerAction.h"
#include "AI/Sense/AISense_PlayerAction.h"

UAISenseConfig_PlayerAction::UAISenseConfig_PlayerAction()
{
	DebugColor = FColor::Purple;
}

TSubclassOf<UAISense> UAISenseConfig_PlayerAction::GetSenseImplementation() const
{
	return UAISense_PlayerAction::StaticClass();
}
