// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/ActionComponent/ActionComponent.h"
#include "MonsterActionComponent.generated.h"

struct FMonsterData;
struct FHitInfo;
enum class EGimicType : uint8;

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
	FMonsterData* GetData() { return Data; }

public:
	virtual void Init(FTableRowBase* _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp) override;
	
	float PlayAttackAction();

	TObjectPtr<UAnimMontage> GetCurrentMontage() { return CurAttackMontage; }

	void SetCurAttackIdx(uint8 _idx) { CurAttackIdx = _idx; }
	uint8 GetCurAttackIdx() { return CurAttackIdx; }
};


UCLASS()
class ARPG_HUNTER_API UBossActionComponent : public UMonsterActionComponent
{
	GENERATED_BODY()

private:
	enum EState : uint8
	{
		NORMAL,
		GIMIC,
		GROGGY,
	};

	EState CurState;
	EGimicType CurGimicType;
	uint16 GimicValue;

	bool InterruptCounter(const FHitInfo& _hitInfo);
	bool InterruptStagger(const FHitInfo& _hitInfo);

public:
	virtual void Init(FTableRowBase* _data, TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp) override;

	bool StartGimic(EGimicType _type, uint16 _gimicValue);
	void InterruptGimic(const FHitInfo& _hitInfo);

	bool IsInGimic() const { return CurState == GIMIC; }
};