// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/Enum.h"
#include "StatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, uint16, uint16)

/*
* 
*/
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHitEvent, bool&)

class UEffect;
struct FEffectParam;

USTRUCT()
struct FCharacterResource
{
	GENERATED_BODY()
public:
	uint32 MaxValue;
	uint32 Value;
	FOnValueChanged OnValueChanged;

	FCharacterResource() : MaxValue(100), Value(100) {}

	void Init(uint32 _max, bool _bFull = true);
	void InvokeDelegate() 
	{
		OnValueChanged.Broadcast(Value, MaxValue);
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UStatComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterStatType, uint32> Stat;
	UPROPERTY(VisibleAnywhere, Category = "Resource", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterResourceType, FCharacterResource> Resource;

	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	uint16 StaminaRecoveryPerSecond{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryRate{ 0.1f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryPauseTime{ 1.0f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float PenaltyTimeOnStaminaExhaustion{ 5.0f };

	FTimerHandle StaminaRecoveryTimer;

	// 효과 관리용 컨테이너 : 이펙트 -> 타이머 핸들 찾기
	UPROPERTY()
	TMap<TObjectPtr<UEffect>, FTimerHandle> MapEffect;
	// 효과로 얻은 스탯
	UPROPERTY(VisibleAnywhere, Category = "Stat|Effect", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterStatType, uint32> EffectedStat;

	FCharacterResource& GetResource(ECharacterResourceType _type) 
	{ 
		Resource[_type];
		return Resource[_type]; 
	}

public:	
	FOnHitEvent OnHitEvent; // 피격 이벤트

	void Init(const TMap<ECharacterStatType, uint32>& _initStat);
	void Clear();

	uint32 GetStat(ECharacterStatType _type) const { return Stat[_type] + EffectedStat[_type]; }
	uint32 GetResourceValue(ECharacterResourceType _type) const { return Resource[_type].Value; }
	uint32 GetResourceMaxValue(ECharacterResourceType _type) const { return Resource[_type].MaxValue; }
	FOnValueChanged& GetResourceEvent(ECharacterResourceType _type) { return Resource[_type].OnValueChanged; }
	bool TryUseResource(ECharacterResourceType _type, uint32 _amount);
	void RecoverResource(ECharacterResourceType _type, uint32 _amount);

	bool IsDead() 
	{ 
		return GetResource(ECharacterResourceType::HEALTH).Value == 0;
	}
	bool TakeDamage(uint32 _damage);

	bool IsStaggering() 
	{ 
		return GetResource(ECharacterResourceType::STAMINA).Value == 0;
	}
	void TakeStaminaDamage(uint32 _damage);
	bool TryUseStamina(uint32 _amount);
	void StartStaminaRecovery();
	void PauseAndRestartStaminaRecovery(float _pauseSecond);

	void ApplyEffect(TSubclassOf<UEffect> _effectClass, FEffectParam* _effectParam);
	void RegisterEffect(TObjectPtr<UEffect> _effect);
	void RemoveEffect(TObjectPtr<UEffect> _effect);

	void AddStat(ECharacterStatType _type, uint32 _amount) { EffectedStat[_type] += _amount; }
	void SubStat(ECharacterStatType _type, uint32 _amount) { EffectedStat[_type] -= _amount; }
};
