// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/Effectable.h"
#include "Interface/AttackNotifyHandler.h"
#include "MonsterBase.generated.h"

enum class EMonsterType : uint8;
struct FMonsterData;

DECLARE_DELEGATE(FOnAttackMontageEnded);

UCLASS()
class ARPG_HUNTER_API AMonsterBase : public ACharacter, public IHitable, public IEffectable, public IAttackNotifyHandler
{
	GENERATED_BODY()

public:
	AMonsterBase();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStatComponent> StatComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> WidgetComp;

	TObjectPtr<UAnimInstance> AnimInstance;
	
	FMonsterData* Data; 
	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBehaviorTree> MonsterBT;
	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBlackboardData> MonsterBB;

	int CurAttackMontageIdx{0};

protected:
	virtual void BeginPlay() override;

	TObjectPtr<UWidgetComponent> GetWidgetComp() { return WidgetComp; }
	void SetBehaviorTree(TObjectPtr<UBehaviorTree> _inBT) { MonsterBT = _inBT; }
	void SetBlackboardData(TObjectPtr<UBlackboardData> _inBB) { MonsterBB = _inBB; }

	UFUNCTION()
	virtual void OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted);

	void SetWalkable(bool _bIsWalkable);

public:
	FOnAttackMontageEnded OnAttackMontageEnded;

	virtual void Init(FMonsterData* _data);
	virtual void Attack();
	
	// IAttackNotifyHandler을(를) 통해 상속됨
	virtual void HandleAttackNotify(uint8 _opt) override;
	// IHitable을(를) 통해 상속됨
	virtual void HitBy(const FHitInfo& _hitInfo) override;

	bool IsDead();
	virtual void OnDead();

	TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return MonsterBT; }
	TObjectPtr<UBlackboardData> GetBlackboardData() const { return MonsterBB; }

	float GetRecognitionRange() const;
	float GetAttackRange() const;
	float GetMoveSpeed() const;

	TObjectPtr<UAnimMontage> GetHitMontage() const;
	TObjectPtr<UAnimMontage> GetAttackMontage(int _idx = 0) const;

	// IEffectable을(를) 통해 상속됨
	void ApplyEffect(TSubclassOf<class UEffect> _effectClass, FEffectParam* _effectParam) override;
	void KnockBack(const FHitInfo& _hitInfo);
};
