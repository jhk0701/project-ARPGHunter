// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ActionComponent/ActionComponent.h"
#include "MonsterActionComponent.generated.h"

struct FMonsterData;
struct FMonsterAction;
struct FHitInfo;

enum class EMonsterAttackType : uint8;
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
	TArray<float> ActionTotalWeights;

protected:
	FMonsterData* GetData() const { return Data; }

	TWeakObjectPtr<class UAction> GetCurrentAction() const override;
	void PostProcessAttack(uint8 _opt, const TArray<FHitResult>& _inHitResults) override;

public:
	virtual void Init(FTableRowBase* _data, TWeakObjectPtr<UAnimInstance> _ownerAnimInstance, TWeakObjectPtr<USkeletalMeshComponent> _firePointComp);
	bool IsValid() const override { return nullptr != Data; }

	float PlayAttackAction(const FName& _opt = NAME_None);
	virtual void PlayHitAction(EMonsterState _state);
	bool PlayExtraAction(const FName& _actName);

	void SelectAttack(EMonsterAttackType _type);
	void SetCurAttackIdx(uint8 _idx) { CurAttackIdx = _idx; }
	uint8 GetCurAttackIdx() const { return CurAttackIdx; }

	const FMonsterAction& GetCurrentMonsterAction() const;
	TObjectPtr<UAnimMontage> GetCurrentMontage() const;
	uint16 GetAttackActionDamagePer(uint8 _opt);
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