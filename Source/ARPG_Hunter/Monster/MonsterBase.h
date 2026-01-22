// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
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
class ARPG_HUNTER_API AMonsterBase : public ACharacter, public IHitable, public IAttackNotifyHandler
{
	GENERATED_BODY()

public:
	AMonsterBase();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStatComponent> StatComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> WeaponComp;

	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBehaviorTree> MonsterBT;
	UPROPERTY(EditAnywhere, Category = "AI|BT")
	TObjectPtr<class UBlackboardData> MonsterBB;

	TObjectPtr<UAnimInstance> AnimInstance;

#pragma region TmpData
	
	UPROPERTY(EditAnywhere, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "AI|Param")
	float RecognitionRange{ 1000.0f };
	UPROPERTY(EditAnywhere, Category = "AI|Param")
	float AttackRange{ 150.0f };

#pragma endregion

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	void SetBehaviorTree(TObjectPtr<UBehaviorTree> _inBT) { MonsterBT = _inBT; }
	void SetBlackboardData(TObjectPtr<UBlackboardData> _inBB) { MonsterBB = _inBB; }

	UFUNCTION()
	virtual void OnAnimMontageEnd(UAnimMontage* _montage, bool _bInterrupted);

public:
	FOnAttackMontageEnded OnAttackMontageEnded;

	virtual void Attack();
	// IAttackNotifyHandler을(를) 통해 상속됨
	virtual void HandleAttackNotify() override;

	// IHitable을(를) 통해 상속됨
	virtual void HitBy(uint16 _damage) override; 
	void OnTakeDamage(uint16 _remainHp, uint16 _maxHp);

	bool IsDead();
	TObjectPtr<UBehaviorTree> GetBehaviorTree() { return MonsterBT; }
	TObjectPtr<UBlackboardData> GetBlackboardData() { return MonsterBB; }

	float GetRecognitionRange() { return RecognitionRange; }
	float GetAttackRange() { return AttackRange; }
};
