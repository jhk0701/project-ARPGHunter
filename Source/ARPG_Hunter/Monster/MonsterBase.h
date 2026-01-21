// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/AttackNotifyHandler.h"
#include "MonsterBase.generated.h"

UCLASS()
class ARPG_HUNTER_API AMonsterBase : public ACharacter, public IHitable, public IAttackNotifyHandler
{
	GENERATED_BODY()

public:
	AMonsterBase();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStatComponent> StatComp;
	
	UPROPERTY(EditAnywhere, Category = "Animation|Montage")
	TObjectPtr<UAnimMontage> HitMontage;


protected:
	virtual void BeginPlay() override;

public:	
	// virtual void Tick(float DeltaTime) override;
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void OnTakeDamage(uint16 _remainHp, uint16 _maxHp);

	// IHitable을(를) 통해 상속됨
	void HitBy(uint16 _damage) override;

	// IAttackNotifyHandler을(를) 통해 상속됨
	void HandleAttackNotify() override;

	bool IsDead();
};
