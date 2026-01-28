// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

class UEffect;
struct FEffectParam;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatValueChanged, uint16, uint16)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitEvent, bool&)

USTRUCT()
struct FStat 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint16 Attack{ 10 };
	UPROPERTY(EditAnywhere)
	uint16 Defense{ 10 };
	UPROPERTY(EditAnywhere)
	uint8 CriticalPer{ 20 }; // 크리티컬 확률
	UPROPERTY(EditAnywhere)
	uint8 CriticalDamagePer{ 100 }; // 크리티컬 시, 증가 데미지
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	FStat Stat;

	UPROPERTY(EditAnywhere, Category = "Stat|Health", meta = (AllowPrivateAccess = "true"))
	uint16 MaxHealth{ 100 };
	UPROPERTY(VisibleAnywhere, Category = "Stat|Resource")
	uint16 Health{ 100 };

	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	uint16 MaxStamina{ 100 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	uint16 StaminaRecoveryPerSecond{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryRate{ 0.1f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryPauseTime{ 1.0f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float PenaltyTimeOnStaminaExhaustion{ 5.0f };
	UPROPERTY(VisibleAnywhere, Category = "Stat|Resource")
	uint16 Stamina{ 100 };

	FTimerHandle StaminaRecoveryTimer;
	void StartStaminaRecovery();

	UPROPERTY(EditAnywhere, Category = "Stat|Skill", meta = (AllowPrivateAccess = "true"))
	uint8 MaxSkill{ 100 };
	UPROPERTY(VisibleAnywhere, Category = "Stat|Resource")
	uint8 Skill{ 0 };
	
	// 효과 관리용 컨테이너 : 이펙트 -> 타이머 핸들 찾기
	UPROPERTY()
	TMap<TObjectPtr<UEffect>, FTimerHandle> MapEffect;

	// 효과로 얻은 스탯
	UPROPERTY(VisibleAnywhere, Category = "Stat|Effect", meta = (AllowPrivateAccess = "true"))
	FStat EffectedStat;
	
public:	
	FOnStatValueChanged OnHealthChanged;
	FOnStatValueChanged OnStaminaChanged;
	FOnStatValueChanged OnSkillChanged;

	FOnHitEvent OnHitEvent; // 피격 이벤트

	void Init();

	uint16 GetAttack() { return Stat.Attack + EffectedStat.Attack; }
	uint16 GetDefense() { return Stat.Defense + EffectedStat.Defense; }
	uint8 GetCriticalPer() { return FMath::Min(100, Stat.CriticalPer + EffectedStat.CriticalPer); }
	uint8 GetCriticalDamagePer() { return Stat.CriticalDamagePer + EffectedStat.CriticalDamagePer; }

	uint16 GetMaxHealth() { return MaxHealth; }
	uint16 GetHealth() { return Health; }
	bool IsDead() { return Health == 0; }
	bool TakeDamage(uint16 _damage);
	void RecoverHealth(uint16 _amount);

	uint16 GetMaxStamina() { return MaxStamina; }
	uint16 GetStamina() const { return Stamina; }
	bool IsStaggering() { return Stamina == 0; }
	bool TryUseStamina(uint16 _amount);
	void TakeStaminaDamage(uint16 _damage);
	void RecoverStamina(uint16 _amount);
	void PauseAndRestartStaminaRecovery(float _pauseSecond);

	uint8 GetMaxSkill() { return MaxSkill; }
	uint8 GetSkill() { return Skill; }
	bool TryUseSkill(uint8 _amount);
	void RecoverSkill(uint8 _amount);

	void ApplyEffect(TSubclassOf<UEffect> _effectClass, FEffectParam* _effectParam);
	void RegisterEffect(TObjectPtr<UEffect> _effect);
	void RemoveEffect(TObjectPtr<UEffect> _effect);

	void AddAttack(uint16 _amount) { EffectedStat.Attack += _amount; }
	void AddDefense(uint16 _amount) { EffectedStat.Defense += _amount; }
	void AddCritPer(uint8 _amount) { EffectedStat.CriticalPer += _amount; }
	void AddCritDmg(uint8 _amount) { EffectedStat.CriticalDamagePer += _amount; }

	void SubAttack(uint16 _amount) { EffectedStat.Attack -= _amount; }
	void SubDefense(uint16 _amount) { EffectedStat.Defense -= _amount; }
	void SubCritPer(uint8 _amount) { EffectedStat.CriticalPer -= _amount; }
	void SubCritDmg(uint8 _amount) { EffectedStat.CriticalDamagePer -= _amount; }
};
