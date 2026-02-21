// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/Effectable.h"
#include "Interface/AttackNotifyHandler.h"
#include "MonsterBase.generated.h"

enum class EMonsterType : uint8;
enum class EMonsterAttackType : uint8;
enum class EMonsterState : uint8;
struct FMonsterData;
struct FMonsterAction;

struct FMonsterInitParam
{
	FName ID;
	uint8 SectionIndex;
	FVector Location;
	FRotator Rotation;
};

DECLARE_DELEGATE(FOnAttackMontageEnded);
DECLARE_DELEGATE_OneParam(FOnMonsterDead, TObjectPtr<class AMonsterBase>);

UCLASS(Abstract)
class ARPG_HUNTER_API AMonsterBase : public ACharacter, public IHitable, public IEffectable, public IAttackNotifyHandler
{
	GENERATED_BODY()

public:
	AMonsterBase();
private:
	UPROPERTY(VisibleAnywhere, Category = "Data")
	FName ID;
	FMonsterData* Data;
	EMonsterState CurState;

	UPROPERTY(VisibleAnywhere, Category = "Section")
	uint8 SectionID{0};

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStatComponent> StatComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponComp;

	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBehaviorTree> MonsterBT;
	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBlackboardData> MonsterBB;

	UPROPERTY(EditAnywhere, Category = "Monster|Dead")
	float DeadDelay{ 3.0f };
	FTimerHandle OnDeadTimer;

	UPROPERTY(VisibleAnywhere)
	bool bIsMovable{ true };

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMonsterActionComponent> ActionComp;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UFUNCTION()
	virtual void OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted);
	virtual void OnDead();

	// 구체적인 동작은 하위에서 구현할 것
	virtual void ShowDamageUI(bool _bIsCritical, uint32 _damage) {};

	void SetBehaviorTree(TObjectPtr<UBehaviorTree> _inBT) { MonsterBT = _inBT; }
	void SetBlackboardData(TObjectPtr<UBlackboardData> _inBB) { MonsterBB = _inBB; }
	void SetMovable(bool _bIsMovable);
	void SetState(EMonsterState _state) { CurState = _state; }

	uint8 GetSectionID() const { return SectionID; }
	FMonsterData* GetData() const { return Data; }
	
	TObjectPtr<UStatComponent> GetStatComp() { return StatComp; }
	TObjectPtr<USkeletalMeshComponent> GetWeaponComp() { return WeaponComp; }

public:
	FOnAttackMontageEnded OnAttackMontageEnded;
	FOnMonsterDead OnMonsterDead;

	virtual void Init(const FMonsterInitParam& _param);
	virtual float Attack(EMonsterAttackType _type);
	// IAttackNotifyHandler을(를) 통해 상속됨
	virtual void HandleAttackNotify(uint8 _opt) override;
	// IHitable을(를) 통해 상속됨
	virtual void HitBy(const FHitInfo& _hitInfo) override;

	bool IsDead() const;
	bool IsMovable() const { return bIsMovable; }

	TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return MonsterBT; }
	TObjectPtr<UBlackboardData> GetBlackboardData() const { return MonsterBB; }
	EMonsterType GetType() const;
	EMonsterState GetState() const { return CurState; }

	// IEffectable을(를) 통해 상속됨
	void ApplyEffect(TObjectPtr<class UEffectData> _effectData) override;

	TWeakObjectPtr<AActor> GetTarget() const;
};
