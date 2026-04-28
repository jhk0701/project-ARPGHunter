// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/TermEffect/TermEffect.h"
#include "EventEffect.generated.h"

enum class EHitOption : uint8;

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UEventEffect : public UTermEffect
{
	GENERATED_BODY()
private:
	FDelegateHandle EventHandle;
protected:
	virtual FDelegateHandle Subscribe() { return FDelegateHandle(); } // 하위 클래스에서 재정의해줄 것
public:
	virtual bool Activate() override;
	virtual void Deactivate() override;
};

UCLASS(Abstract)
class ARPG_HUNTER_API UOnHitEffect : public UEventEffect
{
	GENERATED_BODY()
protected:
	FDelegateHandle Subscribe() override;
public:
	/// <summary>
	/// 버프를 가진 대상이 피격시 호출될 메서드
	/// </summary>
	/// <param name="_outBool">true : 피격 무효, false : 피격 진행</param>
	virtual void OnHitEvent(uint8& _hitBit, uint32& _damage) {};
};


UCLASS()
class ARPG_HUNTER_API UInvincibleEffect : public UOnHitEffect
{
	GENERATED_BODY()
public:
	void OnHitEvent(uint8& _hitBit, uint32& _damage) override;
};

UCLASS()
class ARPG_HUNTER_API UJustDodgeEffect : public UOnHitEffect
{
	GENERATED_BODY()
public:
	void OnHitEvent(uint8& _hitBit, uint32& _damage) override;
};

UCLASS()
class ARPG_HUNTER_API USuperArmorEffect : public UOnHitEffect
{
	GENERATED_BODY()
public:
	void OnHitEvent(uint8& _hitBit, uint32& _damage) override;
};
