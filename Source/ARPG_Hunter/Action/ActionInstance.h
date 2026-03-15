// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Define/InnerTypes.h"
#include "ActionInstance.generated.h"

class UAction;
class UEffectData;
enum class EActionEvent : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UActionInstance : public UObject
{
	GENERATED_BODY()
private:
	TObjectPtr<UAction> Action;

	// 스킬 반영 수치
	uint16 AddictiveAttackDamagePer{0};
	uint16 AddictiveStaggerDamage{0};
	uint8 ReduceStaminaUsage{0};

	TMap<UClass*, uint32> AddictiveEffectValue;
	UPROPERTY()
	TMap<EActionEvent, FActionEventEffect> EventEffect;
	
public:
	void SetAction(TObjectPtr<UAction> _action);
	TObjectPtr<UAction> GetAction() const { return Action; }

	void AddAttackDamagePer(uint16 _value) { AddictiveAttackDamagePer += _value; }
	void AddStaggerDamage(uint16 _value) { AddictiveStaggerDamage += _value; }
	void AddReduceStaminaUsage(uint8 _value) { ReduceStaminaUsage += _value; }

	uint16 GetAttackDamagePer(uint8 _opt) const;
	uint16 GetStaggerDamage(uint8 _opt) const;
	uint8 GetStaminaUsage() const;

	void AddEffectValue(UClass* _effectClass, uint32 _value);
	uint32 GetAddictiveEffectValue(TObjectPtr<UEffectData> _effect);

	void ExtendEventEffect(EActionEvent _eventType, TObjectPtr<UEffectData> _effect);

	bool IsContainEventEffect(EActionEvent _eventType) const { return EventEffect.Contains(_eventType); }
	const TArray<TObjectPtr<UEffectData>>& GetEventEffect(EActionEvent _eventType) const { return EventEffect[_eventType].Effects; };
};
