// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/Effectable.h"
#include "Interface/AttackNotifyHandler.h"
#include "MonsterBase.generated.h"

UENUM(BlueprintType)
enum class EMonsterType : uint8 
{
	MELEE		UMETA(DisplayName = "Melee"),
	RANGED		UMETA(DisplayName = "Ranged"),
	BOSS		UMETA(DisplayName = "Boss"),

	END			UMETA(DisplayName = "End")
};

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

#pragma region TmpData

	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBehaviorTree> MonsterBT;
	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBlackboardData> MonsterBB;

	UPROPERTY(EditAnywhere, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "AI|Param")
	float RecognitionRange{ 1000.0f };
	UPROPERTY(EditAnywhere, Category = "AI|Param")
	float AttackRange{ 150.0f };
	UPROPERTY(EditAnywhere, Category = "AI|Param")
	float Speed{200.0f};

#pragma endregion

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	TObjectPtr<UWidgetComponent> GetWidgetComp() { return WidgetComp; }
	void SetBehaviorTree(TObjectPtr<UBehaviorTree> _inBT) { MonsterBT = _inBT; }
	void SetBlackboardData(TObjectPtr<UBlackboardData> _inBB) { MonsterBB = _inBB; }

	UFUNCTION()
	virtual void OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted);

	void SetWalkable(bool _bIsWalkable);


public:
	FOnAttackMontageEnded OnAttackMontageEnded;

	virtual void Attack();
	// IAttackNotifyHandler을(를) 통해 상속됨
	virtual void HandleAttackNotify(uint8 _opt) override;

	// IHitable을(를) 통해 상속됨
	virtual void HitBy(const FHitInfo& _hitInfo) override;

	bool IsDead();
	virtual void OnDead();

	TObjectPtr<UBehaviorTree> GetBehaviorTree() { return MonsterBT; }
	TObjectPtr<UBlackboardData> GetBlackboardData() { return MonsterBB; }

	float GetRecognitionRange() { return RecognitionRange; }
	float GetAttackRange() { return AttackRange; }

	// IEffectable을(를) 통해 상속됨
	void ApplyEffect(TSubclassOf<class UEffect> _effectClass, FEffectParam* _effectParam) override;
	void KnockBack(const FHitInfo& _hitInfo);
};
