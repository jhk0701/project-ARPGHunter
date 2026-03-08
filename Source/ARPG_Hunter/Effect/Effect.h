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
	float AddPercent;		// 스킬 성장 등에 따른 증가 퍼센트
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
	TWeakObjectPtr<UStatComponent> TargetComp; // 효과 대상 : 약참조 소유
	FEffectParam* BaseParam;
	float AddPercent;
	uint8 Stack;

protected:
	bool IsValid() { return TargetComp.IsValid() && BaseParam != nullptr; }
	TWeakObjectPtr<UStatComponent> GetTarget() { return TargetComp; }
	FEffectParam* GetParam() { return BaseParam; }

public:
	virtual void Init(UStatComponent* _target, FEffectContext* _context) 
	{
		TargetComp = _target;
		DataPointer = _context->DataPointer;
		BaseParam = _context->Param;
		AddPercent = _context->AddPercent;
		Stack = 1;
	}
	//하위 클래스에서 구체적 동작 구현
	virtual bool Activate() { return true; }; // 활성화 시 결과 반환
	virtual void Deactivate() {};
	UObject* GetID() { return DataPointer; }
	float GetDuration();
	uint8 GetMaxStack();
	uint8 GetStack() { return Stack; }
	bool IsStackFull();
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