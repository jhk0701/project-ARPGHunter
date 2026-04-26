// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Sense/AISenseEvent_PlayerAction.h"
#include "AI/Sense/AISense_PlayerAction.h"

FAISenseID UAISenseEvent_PlayerAction::GetSenseID() const
{
	// 부모 클래스에서는 Pure Virtual 선언이고 기본적으로 Invalid 반환이므로 재정의해서 SenseID 반환
	return UAISense::GetSenseID<UAISense_PlayerAction>();
}
