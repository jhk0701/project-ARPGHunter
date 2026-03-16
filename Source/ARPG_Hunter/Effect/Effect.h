// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/StatComponent.h"
#include "Effect.generated.h"

struct FEffectParam;
struct FEffectContext 
{
	UObject* DataPointer;	// 이펙트 식별용 UEffectData의 주소값
	FEffectParam* Param;
	uint32 AddictiveValue;
};

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API UEffect : public UObject
{
	GENERATED_BODY()

private:
	UObject* DataPointer;
	TWeakObjectPtr<AActor> Subject; // 효과 부여 주체 : 약참조 소유
	TWeakObjectPtr<UStatComponent> TargetComp; // 효과 대상 : 약참조 소유
	FEffectParam* BaseParam;
	uint32 AddictiveValue;
	uint8 Stack;

protected:
	bool IsValid() { return TargetComp.IsValid() && BaseParam != nullptr; }
	TWeakObjectPtr<AActor> GetSubject() { return Subject; }
	TWeakObjectPtr<UStatComponent> GetTarget() { return TargetComp; }

public:
	virtual void Init(TWeakObjectPtr<AActor> _subject, TWeakObjectPtr<UStatComponent> _target, FEffectContext* _context)
	{
		Subject = _subject;
		TargetComp = _target;
		DataPointer = _context->DataPointer;
		BaseParam = _context->Param;
		AddictiveValue = _context->AddictiveValue;
		Stack = 1;
	}
	
	//하위 클래스에서 구체적 동작 구현
	virtual bool Activate() { return true; }; // 활성화 시 결과 반환
	virtual void Deactivate() {};

	UObject* GetID() { return DataPointer; }
	uint32 GetValue() const;
	float GetDuration() const;
	float GetRepeatInterval() const;
	const TArray<TObjectPtr<class UEffectData>>& GetTargetEffect() const;
	
	uint8 GetMaxStack() const;
	uint8 GetStack() const { return Stack; }
	bool IsStackFull() const;
	void AddStack();
};

UCLASS()
class ARPG_HUNTER_API URecoverHealth : public UEffect 
{
	GENERATED_BODY()
public:
	virtual bool Activate() override;
};

UCLASS()
class ARPG_HUNTER_API URecoverSkill : public UEffect
{
	GENERATED_BODY()
public:
	virtual bool Activate() override;
};

UCLASS()
class ARPG_HUNTER_API URecoverStamina : public UEffect
{
	GENERATED_BODY()
public:
	virtual bool Activate() override;
};

UCLASS()
class ARPG_HUNTER_API UAddEffectUsingSkill : public UEffect
{
	GENERATED_BODY()
public:
	virtual bool Activate() override;
};

UCLASS()
class ARPG_HUNTER_API UDamageUsingEffect : public UEffect
{
	GENERATED_BODY()
public:
	virtual bool Activate() override;
};