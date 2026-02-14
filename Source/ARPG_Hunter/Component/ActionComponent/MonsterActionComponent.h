// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ActionComponent/ActionComponent.h"
#include "MonsterActionComponent.generated.h"

struct FMonsterData;
struct FHitInfo;

enum class EGimicType : uint8;
enum class EMonsterState : uint8;

DECLARE_DELEGATE_OneParam(FGimicEvent, EGimicType);
DECLARE_DELEGATE_TwoParams(FOnGimicValueChanged, uint16, uint16);

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UMonsterActionComponent : public UActionComponent
{
	GENERATED_BODY()

private:
	FMonsterData* Data;

	uint8 CurAttackIdx{ 0 };
	TObjectPtr<UAnimMontage> CurAttackMontage{ nullptr };

protected:
	FMonsterData* GetData() const { return Data; }

public:
	virtual void Init(FTableRowBase* _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp);
	
	float PlayAttackAction();
	virtual void PlayHitAction(EMonsterState _state);

	TObjectPtr<UAnimMontage> GetCurrentMontage() const { return CurAttackMontage; }

	void SetCurAttackIdx(uint8 _idx) { CurAttackIdx = _idx; }
	uint8 GetCurAttackIdx() const { return CurAttackIdx; }
	
};


UCLASS()
class ARPG_HUNTER_API UBossActionComponent : public UMonsterActionComponent
{
	GENERATED_BODY()

private:
	EGimicType CurGimicType;
	uint16 GimicMaxValue;
	uint16 GimicValue;

	bool InterruptCounter(const FHitInfo& _hitInfo);
	bool InterruptStagger(const FHitInfo& _hitInfo);

public:
	FGimicEvent OnGimicStart;
	FGimicEvent OnGimicEnd;
	FOnGimicValueChanged OnGimicValueChanged;

	virtual void PlayHitAction(EMonsterState _state) override;

	bool StartGimic(EGimicType _type, uint16 _gimicValue);
	void InterruptGimic(const FHitInfo& _hitInfo);
	void EndGimic();

	uint16 GetGimicMaxValue() const { return GimicMaxValue; }
	uint16 GetGimicValue() const { return GimicValue; }
};