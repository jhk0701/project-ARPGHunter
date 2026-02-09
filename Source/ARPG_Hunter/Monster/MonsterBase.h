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

struct FMonsterInitParam
{
	FName ID;
	uint8 SectionIndex;
	FVector Location;
	FRotator Rotation;
};

DECLARE_DELEGATE(FOnAttackMontageEnded);
DECLARE_DELEGATE_OneParam(FOnDead, TObjectPtr<class AMonsterBase>);

UCLASS()
class ARPG_HUNTER_API AMonsterBase : public ACharacter, public IHitable, public IEffectable, public IAttackNotifyHandler
{
	GENERATED_BODY()

public:
	AMonsterBase();
private:
	UPROPERTY(VisibleAnywhere, Category = "Data")
	FName ID;
	FMonsterData* Data;

	UPROPERTY(VisibleAnywhere, Category = "Section")
	uint8 SectionID{0};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStatComponent> StatComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponComp;
	UPROPERTY()
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBehaviorTree> MonsterBT;
	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBlackboardData> MonsterBB;

	int CurAttackMontageIdx{0};
	TObjectPtr<UAnimMontage> CurAttackMontage;

	UPROPERTY(EditAnywhere, Category = "Monster|Dead")
	float DeadDelay{ 3.0f };
	FTimerHandle OnDeadTimer;

	UPROPERTY(VisibleAnywhere)
	bool bIsMovable{ true };

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	 
	UFUNCTION()
	virtual void OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted);
	virtual void OnDead();
	virtual void ShowDamageUI(bool _bIsCritical, uint32 _damage) {};

	void SetBehaviorTree(TObjectPtr<UBehaviorTree> _inBT) { MonsterBT = _inBT; }
	void SetBlackboardData(TObjectPtr<UBlackboardData> _inBB) { MonsterBB = _inBB; }
	void SetMovable(bool _bIsMovable);

	uint8 GetSectionID() const { return SectionID; }
	uint8 GetCurAttackIdx() const { return CurAttackMontageIdx; }
	FMonsterData* GetData() const { return Data; }
	TObjectPtr<UStatComponent> GetStatComp() { return StatComp; }
	TObjectPtr<USkeletalMeshComponent> GetWeaponComp() { return WeaponComp; }
	

public:
	FOnAttackMontageEnded OnAttackMontageEnded;
	FOnDead OnMonsterDead;

	virtual void Init(const FMonsterInitParam& _param);
	virtual void Attack();

	// IAttackNotifyHandler을(를) 통해 상속됨
	virtual void HandleAttackNotify(uint8 _opt) override {};
	// IHitable을(를) 통해 상속됨
	virtual void HitBy(const FHitInfo& _hitInfo) override;

	bool IsDead();
	
	TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return MonsterBT; }
	TObjectPtr<UBlackboardData> GetBlackboardData() const { return MonsterBB; }
	EMonsterType GetType() const;
	TObjectPtr<UAnimMontage> GetHitMontage() const;
	TObjectPtr<UAnimMontage> GetAttackMontage(int _idx = 0) const;

	// IEffectable을(를) 통해 상속됨
	void ApplyEffect(TObjectPtr<class UEffectData> _effectData) override;
};
