// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Define/Enum.h"
#include "StatComponent.generated.h"


class UEffectData;
class UEffect;

enum class EHitOption : uint8
{
	NONE,			// 옵션 없음
	IMMUNE_HIT		= 0b001,	// 피격 면역 : 피격 자체 무효
	IMMUNE_STIFFEN	= 0b010,	// 경직 면역 : 피격 모션만 무효
	IMMUNE_DEBUF	= 0b100,	// 디버프 면역 : 디버프 적용 무효
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, uint16, uint16)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHitEvent, uint8&, uint32&)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEffectRegistered, UObject*, UTexture2D*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEffectRemoved, UObject*);
DECLARE_MULTICAST_DELEGATE(FOnDead);

USTRUCT()
struct FCharacterResource
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	uint32 MaxValue;
	UPROPERTY(VisibleAnywhere)
	uint32 Value;
	FOnValueChanged OnValueChanged;

	FCharacterResource() : MaxValue(100), Value(100) {}

	void Init(uint32 _max, bool _bFull = true);
	void InvokeDelegate() 
	{
		OnValueChanged.Broadcast(Value, MaxValue);
	}
};

USTRUCT()
struct FAppliedEffect 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UEffect> Effect;
	FTimerHandle Timer;

	FAppliedEffect() : Effect(nullptr){}
	FAppliedEffect(TObjectPtr<UEffect> _newEffect) : Effect(_newEffect) {}
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
	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterStatType, uint32> EquipmentStat;
	UPROPERTY(VisibleAnywhere, Category = "Stat", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterResourceType, FCharacterResource> Resource;

	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	uint16 StaminaRecoveryPerSecond{ 10 };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryRate{ 0.1f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoveryPauseTime{ 1.0f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float PenaltyTimeOnStaminaExhaustion{ 5.0f };
	UPROPERTY(EditAnywhere, Category = "Stat|Stamina", meta = (AllowPrivateAccess = "true"))
	float StaminaRecoverOnStagger{ 0.5f };

	FTimerHandle StaminaRecoveryTimer;

	// 효과 관리용 컨테이너 : 이펙트 -> 타이머 핸들 찾기
	UPROPERTY()
	TMap<TObjectPtr<UObject>, FAppliedEffect> MapEffect;

	// 효과로 얻은 스탯
	UPROPERTY(VisibleAnywhere, Category = "Stat|Effect", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterStatType, uint32> BuffedStat;
	UPROPERTY(VisibleAnywhere, Category = "Stat|Effect", meta = (AllowPrivateAccess = "true"))
	TMap<ECharacterStatType, uint32> DebuffedStat;

	FCharacterResource& GetResource(ECharacterResourceType _type) 
	{ 
		Resource[_type];
		return Resource[_type]; 
	}

public:	
	FOnHitEvent OnHitEvent; // 피격 이벤트
	FOnEffectRegistered OnEffectRegistered; // 이펙트 등록 이벤트
	FOnEffectRemoved OnEffectRemoved;		// 이펙트 제거 이벤트
	FOnDead OnDead;

	void Init(const TMap<ECharacterStatType, uint32>& _initStat);
	void Init(const TMap<ECharacterStatType, uint32>& _initStat, const TMap<ECharacterStatType, uint32>& _equipmentStat);
	void Clear();
 
	uint32 GetStat(ECharacterStatType _type, bool _bExceptEffect = false) const;
	uint32 GetResourceValue(ECharacterResourceType _type) const { return Resource[_type].Value; }
	uint32 GetResourceMaxValue(ECharacterResourceType _type) const { return Resource[_type].MaxValue; }
	FOnValueChanged& GetResourceEvent(ECharacterResourceType _type) { return Resource[_type].OnValueChanged; }
	bool TryUseResource(ECharacterResourceType _type, uint32 _amount);
	void RecoverResource(ECharacterResourceType _type, uint32 _amount);

	bool IsDead() 
	{ 
		return GetResource(ECharacterResourceType::HEALTH).Value == 0;
	}
	bool TakeDamage(uint32 _damage, TFunction<void()> _stiffAction = nullptr);

	bool IsStaggering() 
	{ 
		return GetResource(ECharacterResourceType::STAMINA).Value == 0;
	}
	void TakeStaminaDamage(uint32 _damage);
	bool TryUseStamina(uint32 _amount);
	void StartStaminaRecovery();
	void PauseAndRestartStaminaRecovery(float _pauseSecond, bool _bIsLoop = true);

	void ApplyEffect(const struct FApplyEffectParam& _param);
	bool RegisterEffect(TObjectPtr<UEffect> _effect);
	void RemoveEffect(TObjectPtr<UEffect> _effect);
	TWeakObjectPtr<UEffect> GetAppliedEffect(TObjectPtr<UObject> _key);

	void AddStat(ECharacterStatType _type, uint32 _amount) { BuffedStat[_type] += _amount; }
	void SubStat(ECharacterStatType _type, uint32 _amount) 
	{
		check(BuffedStat[_type] >= _amount);
		BuffedStat[_type] -= _amount;
	}

	void DebuffStat(ECharacterStatType _type, uint32 _amount) { DebuffedStat[_type] += _amount; }
	void RemoveDebuffStat(ECharacterStatType _type, uint32 _amount) 
	{
		check(DebuffedStat[_type] >= _amount);
		DebuffedStat[_type] -= _amount; 
	}

	bool CheckHitOptionMask(uint8 _hitOpt, uint8 _mask) { return _hitOpt & _mask; }
};
