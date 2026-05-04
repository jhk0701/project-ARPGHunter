// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ARPGCharacterBase.h"
#include "MonsterBase.generated.h"

enum class EMonsterType : uint8;
enum class EMonsterAttackType : uint8;
enum class EMonsterState : uint8; // 행동 상태
enum class EMonsterAlertState : uint8; // 경계 상태
enum class EPlayerActionType : uint8; // 몬스터가 인식하는 플레이어의 액션 타입
struct FMonsterData;
struct FMonsterAction;

struct FMonsterInitParam
{
	FName ID;
	uint32 Lv;
	uint8 SectionIndex;
	FVector Location;
	FRotator Rotation;
};

DECLARE_DELEGATE(FOnActionMontageEnded);
DECLARE_DELEGATE_OneParam(FOnMonsterDead, TObjectPtr<class AMonsterBase>);

UCLASS(Abstract)
class ARPG_HUNTER_API AMonsterBase : public AARPGCharacterBase
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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponComp;

	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBehaviorTree> MonsterBT;
	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBlackboardData> MonsterBB;

	UPROPERTY(EditAnywhere, Category = "Monster|Init")
	float InitDelay{ 0.5f };
	FTimerHandle InitTimer;

	UPROPERTY(EditAnywhere, Category = "Monster|Dead")
	float DeadDelay{ 3.0f };
	FTimerHandle OnDeadTimer;

	UPROPERTY(VisibleAnywhere)
	float MoveSpeed{ 0.0f };
	UPROPERTY(VisibleAnywhere)
	bool bIsMovable{ true };

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TMap<EPlayerActionType, bool> bReactToPlayerAction;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnDead() override;
	UFUNCTION()
	virtual void OnMontageEnded(UAnimMontage* _montage, bool _bInterrupted);
	
	// 구체적인 동작은 하위에서 구현할 것
	virtual void ShowDamageUI(bool _bIsCritical, uint32 _damage) {};

	void SetMovementMode(EMovementMode _mode);

	void SetBehaviorTree(TObjectPtr<UBehaviorTree> _inBT) { MonsterBT = _inBT; }
	void SetBlackboardData(TObjectPtr<UBlackboardData> _inBB) { MonsterBB = _inBB; }
	void SetMovable(bool _bIsMovable);
	void SetState(EMonsterState _state) { CurState = _state; }

	uint8 GetSectionID() const { return SectionID; }
	FMonsterData* GetData() const { return Data; }
	
	TObjectPtr<USkeletalMeshComponent> GetWeaponComp() const { return WeaponComp; }

public:
	FOnActionMontageEnded OnAttackMontageEnded;
	FOnActionMontageEnded OnExtraActMontageEnded;
	FOnMonsterDead OnMonsterDead;

	virtual void Init(const FMonsterInitParam& _param);
	EMonsterType GetType() const;
	EMonsterState GetState() const { return CurState; }
	bool IsMovable() const { return bIsMovable; }
	void SetMoveSpeed(bool _bIsChasing);

#pragma region Monster Action
	
	float Attack(EMonsterAttackType _type, const FName& _opt = NAME_None);
	virtual bool ExtraAct(const FName& _actName);
	// IAttackNotifyHandler을(를) 통해 상속됨
	virtual void HandleAttackNotify(uint8 _opt) override;
	// IHitable을(를) 통해 상속됨
	virtual uint32 HitBy(const FHitInfo& _hitInfo) override;

#pragma endregion
	
#pragma region Monster AI
	TObjectPtr<UBehaviorTree> GetBehaviorTree() const { return MonsterBT; }
	TObjectPtr<UBlackboardData> GetBlackboardData() const { return MonsterBB; }
	TWeakObjectPtr<AActor> GetTarget() const;

	// 하위 클래스에서 구체적인 동작을 구현해둘 것	
	virtual void OnAlertStateChanged(EMonsterAlertState _prevState, EMonsterAlertState _nextState) {}
	// 플레이어 액션에 대한 반응처리
	virtual void TriggerReactForPlayerAction(uint8 _actionType) {}

	void SetReactToPlayerAction(EPlayerActionType _type, bool _newVal) { bReactToPlayerAction[_type] = _newVal; }
	bool GetReactToPlayerAction(EPlayerActionType _type) const { return bReactToPlayerAction[_type]; }
#pragma endregion
	
};
