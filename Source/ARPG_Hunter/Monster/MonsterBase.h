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

#pragma region TmpData
	
	UPROPERTY(EditAnywhere, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "AI")
	float RecognitionRange{ 1000.0f };

#pragma endregion

protected:
	virtual void BeginPlay() override;

	void SetBehaviorTree(TObjectPtr<UBehaviorTree> _inBT) { MonsterBT = _inBT; }
	void SetBlackboardData(TObjectPtr<UBlackboardData> _inBB) { MonsterBB = _inBB; }

public:	
	virtual void Attack();
	void HandleAttackNotify() override; // IAttackNotifyHandler을(를) 통해 상속됨
	
	void HitBy(uint16 _damage) override; // IHitable을(를) 통해 상속됨
	void OnTakeDamage(uint16 _remainHp, uint16 _maxHp);

	bool IsDead();
	
	float GetRecognitionRange() { return RecognitionRange; }

	TObjectPtr<UBehaviorTree> GetBehaviorTree() { return MonsterBT; }
	TObjectPtr<UBlackboardData> GetBlackboardData() { return MonsterBB; }
};
