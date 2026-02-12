// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ActionComponent/ActionComponent.h"
#include "MonsterActionComponent.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UMonsterActionComponent : public UActionComponent
{
	GENERATED_BODY()

private:
	uint8 CurAttackIdx{ 0 };
	TObjectPtr<UAnimMontage> CurAttackMontage;

public:
	float PlayAttackAction();

	TObjectPtr<UAnimMontage> GetCurrentMontage() { return CurAttackMontage; }

	void SetCurAttackIdx(uint8 _idx) { CurAttackIdx = _idx; }
	uint8 GetCurAttackIdx() { return CurAttackIdx; }
};
