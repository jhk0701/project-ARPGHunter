// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/StatComponent.h"

#include "Interface/Effectable.h"
#include "Data/EffectData.h"
#include "Effect/Effect.h"

#include "Define/Debug.h"


void FCharacterResource::Init(uint32 _max, bool _bFull)
{
	MaxValue = _max;
	Value = _bFull ? MaxValue : 0;
}

UStatComponent::UStatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	const uint8 STAT_SIZE = static_cast<uint8>(ECharacterStatType::END);
	BuffedStat.Reserve(STAT_SIZE);
	DebuffedStat.Reserve(STAT_SIZE);
	EquipmentStat.Reserve(STAT_SIZE);
	for (uint8 i = 0; i < STAT_SIZE; ++i)
	{
		ECharacterStatType type = static_cast<ECharacterStatType>(i);

		BuffedStat.Add(type) = 0;
		DebuffedStat.Add(type) = 0;
		EquipmentStat.Add(type) = 0;
	}

	const uint8 RESRC_SIZE = static_cast<uint8>(ECharacterResourceType::END);
	Resource.Reserve(RESRC_SIZE);
	for (uint8 i = 0; i < RESRC_SIZE; ++i)
		Resource.Add(static_cast<ECharacterResourceType>(i)) = FCharacterResource();
}

void UStatComponent::Init(const TMap<ECharacterStatType, uint32>& _initStat)
{
	Stat = _initStat;
	GetResource(ECharacterResourceType::HEALTH).Init(GetStat(ECharacterStatType::HEALTH));
	GetResource(ECharacterResourceType::STAMINA).Init(GetStat(ECharacterStatType::STAMINA));
	GetResource(ECharacterResourceType::SKILL).Init(GetStat(ECharacterStatType::SKILL), false);
}

void UStatComponent::Init(const TMap<ECharacterStatType, uint32>& _initStat, const TMap<ECharacterStatType, uint32>& _equipmentStat)
{
	EquipmentStat = _equipmentStat;
	Init(_initStat);
}

void UStatComponent::Clear()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(StaminaRecoveryTimer))
		TimerManager.ClearTimer(StaminaRecoveryTimer);

	for (TPair<TObjectPtr<UObject>, FAppliedEffect>& pair : MapEffect)
	{
		if (TimerManager.IsTimerActive(pair.Value.Timer))
			TimerManager.ClearTimer(pair.Value.Timer);
	}
}

uint32 UStatComponent::GetStat(ECharacterStatType _type, bool _bExceptEffect) const
{
	uint32 Result = Stat[_type];
	Result += EquipmentStat[_type];

	if (_bExceptEffect)
		return Result;

	Result += BuffedStat[_type];
	if (Result <= DebuffedStat[_type])
		return 0;

	Result -= DebuffedStat[_type];
	return Result;
}

void UStatComponent::StartStaminaRecovery()
{
	TObjectPtr<UWorld> World = GetWorld();
	if (World == nullptr)
		return;

	World->GetTimerManager().SetTimer(
		StaminaRecoveryTimer,
		[this]()
		{
			RecoverResource(ECharacterResourceType::STAMINA, StaminaRecoveryPerSecond * StaminaRecoveryRate);
		},
		StaminaRecoveryRate,
		true
	);
}

bool UStatComponent::TryUseResource(ECharacterResourceType _type, uint32 _amount)
{
	FCharacterResource* pResource = &Resource[_type];

	if (pResource->Value < _amount)
		return false;

	pResource->Value -= _amount;
	pResource->InvokeDelegate();

	return true;
}

void UStatComponent::RecoverResource(ECharacterResourceType _type, uint32 _amount)
{
	FCharacterResource* pResource = &Resource[_type];

	if (pResource->Value == pResource->MaxValue)
		return;

	pResource->Value = FMath::Min<uint32>(pResource->Value + _amount, pResource->MaxValue);
	pResource->InvokeDelegate();
}

bool UStatComponent::TryUseStamina(uint32 _amount)
{
	bool bIsUsed = TryUseResource(ECharacterResourceType::STAMINA, _amount);

	// 스태미나 소진 시, 페널티 시간 부여
	if (bIsUsed)
		PauseAndRestartStaminaRecovery(IsStaggering() ? PenaltyTimeOnStaminaExhaustion : StaminaRecoveryPauseTime);

	return bIsUsed;
}

bool UStatComponent::TakeDamage(uint32 _damage, TFunction<void()> _stiffAction)
{
	if (IsDead())
		return false;
	
	// 피격 발생
	uint8 HitBit = static_cast<uint8>(EHitOption::NONE);
	OnHitEvent.Broadcast(HitBit, _damage); // 피격 시 이벤트 델리게이트 호출

	// 피격 면역 확인
	if (CheckHitOptionMask(HitBit, static_cast<uint8>(EHitOption::IMMUNE_HIT)))
		return false;	// 피격 무효 처리
	
	// 경직 면역 확인
	if (_stiffAction != nullptr && 
		CheckHitOptionMask(HitBit, static_cast<uint8>(EHitOption::IMMUNE_STIFFEN)) == false)
		_stiffAction();  // 경직 동작 수행

	if (TryUseResource(ECharacterResourceType::HEALTH, _damage) == false)
	{
		FCharacterResource& RefResource = GetResource(ECharacterResourceType::HEALTH);
		RefResource.Value = 0;
		RefResource.InvokeDelegate();
	}

	if (IsDead())
		OnDead.Broadcast();

	return true;
}

void UStatComponent::TakeStaminaDamage(uint32 _damage)
{
	if (IsStaggering())
		return;

	if (TryUseResource(ECharacterResourceType::STAMINA, _damage) == false)
	{
		FCharacterResource& RefResource = GetResource(ECharacterResourceType::STAMINA);
		RefResource.Value = 0;
		RefResource.InvokeDelegate();
	}
}


void UStatComponent::PauseAndRestartStaminaRecovery(float _pauseSecond, bool _bIsLoop)
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(StaminaRecoveryTimer))
		TimerManager.ClearTimer(StaminaRecoveryTimer);

	TimerManager.SetTimer(StaminaRecoveryTimer,
		[this]()
		{
			FCharacterResource* pStamina = &Resource[ECharacterResourceType::STAMINA];

			if (IsStaggering())
				RecoverResource(ECharacterResourceType::STAMINA, pStamina->MaxValue * StaminaRecoverOnStagger);
			else
				RecoverResource(ECharacterResourceType::STAMINA, StaminaRecoveryPerSecond * StaminaRecoveryRate);
		},
		StaminaRecoveryRate,
		_bIsLoop,
		_pauseSecond
	);
}

void UStatComponent::ApplyEffect(const FApplyEffectParam& _param)
{
	if(_param.EffectData->bIsDebuff) // 디버프인 경우 확인
	{
		uint8 HitBit = static_cast<uint8>(EHitOption::NONE);
		uint32 DummyDamage = 0;
		OnHitEvent.Broadcast(HitBit, DummyDamage); // 피격 관련 이벤트 확인

		// 피격 면역, 디버프 면역인 경우 return;
		uint8 HitMask = static_cast<uint8>(EHitOption::IMMUNE_HIT) | static_cast<uint8>(EHitOption::IMMUNE_DEBUF);
		if (CheckHitOptionMask(HitBit, HitMask))
			return;
	}

	TObjectPtr<UEffect> EffectInst = NewObject<UEffect>(this, _param.EffectData->Effect);

	FEffectContext Context
	{
		_param.EffectData,
		&_param.EffectData->Param,
		_param.AddictiveValue // 스킬 성장에 따른 효과 증가량
	};
	EffectInst->Init(_param.Subject, this, &Context);
	EffectInst->Activate();
}

bool UStatComponent::RegisterEffect(TObjectPtr<UEffect> _effect)
{
	// 기간형 이펙트 등록
	// 동일 종류 중복 확인
	if (FAppliedEffect* Applied = MapEffect.Find(_effect->GetID())) 
	{
		TObjectPtr<UEffect> AppliedEffect = Applied->Effect;
		
		// 스택 쌓기 가능한지 확인
		bool bIsStacked = false;
		if (false == Applied->Effect->IsStackFull())
		{
			bIsStacked = true;
			AppliedEffect->AddStack(); // 스택 쌓기
		}

		// 지속 시간 갱신
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		TimerManager.ClearTimer(Applied->Timer);
		TimerManager.SetTimer(Applied->Timer, 
			[this, AppliedEffect]() { RemoveEffect(AppliedEffect); },
			AppliedEffect->GetDuration(),
			false);

		return bIsStacked;
	}

	// 신규 효과 추가
	FAppliedEffect& AppliedEffect = MapEffect.Add(_effect->GetID(), FAppliedEffect(_effect));
	GetWorld()->GetTimerManager().SetTimer(
		AppliedEffect.Timer,
		[this, _effect]() { RemoveEffect(_effect); },
		_effect->GetDuration(),
		false);

	TObjectPtr<UEffectData> EffectData = Cast<UEffectData>(_effect->GetID());
	OnEffectRegistered.Broadcast(_effect->GetID(), EffectData->Icon);

	return true;
}

void UStatComponent::RemoveEffect(TObjectPtr<UEffect> _effect)
{
	FAppliedEffect* Applied = MapEffect.Find(_effect->GetID());
	if (nullptr == Applied)
		return;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(Applied->Timer))
		TimerManager.ClearTimer(Applied->Timer);

	_effect->Deactivate();

	MapEffect.Remove(_effect->GetID());
	OnEffectRemoved.Broadcast(_effect->GetID());
}

TWeakObjectPtr<UEffect> UStatComponent::GetAppliedEffect(TObjectPtr<UObject> _key)
{
	FAppliedEffect* Applied = MapEffect.Find(_key);

	if (nullptr == Applied)
		return nullptr;

	return Applied->Effect;
}
