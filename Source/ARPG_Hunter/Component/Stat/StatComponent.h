// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

class UEffect;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatValueChanged, uint16, uint16)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitEvent, bool&)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 MaxHealth{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 Attack{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint16 Defense{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint8 CriticalPer{ 20 }; // 크리티컬 확률
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	uint8 CriticalDamagePer{ 100 }; // 크리티컬 시, 증가 데미지

	UPROPERTY(VisibleAnywhere)
	uint16 Health{ 100 };
	
	// 효과 관리용 컨테이너 : 이펙트 -> 타이머 핸들 찾기
	TMap<TObjectPtr<UEffect>, FTimerHandle> MapEffect;
	// 중복 효과? -> Duration 추가
	// 효과 중복인지 판단 -> UEffect의 ID -> 데이터테이블 기준 ID
	
public:	
	FOnStatValueChanged OnHealthChanged;
	FOnHitEvent OnHitEvent; // 피격 이벤트

	virtual void Init();
	virtual void TakeDamage(uint16 _damage);
	void RecoverHealth(uint16 _amount);

	bool IsDead() { return Health == 0; }
	uint16 GetHealth() { return Health; }
	uint16 GetMaxHealth() { return MaxHealth; }

	uint16 GetAttack() { return Attack; }
	uint16 GetDefense() { return Defense; }

	uint8 GetCriticalPer() { return CriticalPer; }
	uint16 GetCriticalDamagePer() { return CriticalDamagePer; }

	void AddEffect(TObjectPtr<UEffect> _effect);
	void RemoveEffect(TObjectPtr<UEffect> _effect);
};
