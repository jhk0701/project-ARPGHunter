// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GimicAction.generated.h"

enum class EGimicType : uint8;
struct FHitInfo;
struct FGimicParam;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UGimicAction : public UObject
{
	GENERATED_BODY()
	
private:
	EGimicType Type;
	TWeakObjectPtr<AActor> Subject; // 기믹 행둥 주체 약참조

	float ElapsedTime{ 0.0f };
	float LifeTime{ 5.0f };

protected:
	void SetType(EGimicType _type) { Type = _type; }
	void SetLifeTime(float _time) { LifeTime = _time; }

	TWeakObjectPtr<AActor> GetSubject() { return Subject; }
	EGimicType GetType() { return Type; }
	bool IsTimeOut() { return ElapsedTime > LifeTime; }

public:
	virtual void Start(TWeakObjectPtr<AActor> _subject, const FGimicParam& _param);
	virtual void Proceed(float _deltaTime);
	virtual void Interrupt(const FHitInfo& _hitInfo) {}
};

UCLASS()
class ARPG_HUNTER_API UCounterGimic : public UGimicAction 
{
	GENERATED_BODY()

private:
	uint8 EndureCount;

public:
	UCounterGimic();
	void Start(TWeakObjectPtr<AActor> _subject, const FGimicParam& _param) override;
	void Interrupt(const FHitInfo& _hitInfo) override;
};

UCLASS()
class ARPG_HUNTER_API UStaggerGimic : public UGimicAction
{
	GENERATED_BODY()

private:
	uint16 StaggerValue;

public:
	UStaggerGimic();
	void Start(TWeakObjectPtr<AActor> _subject, const FGimicParam& _param) override;
	void Interrupt(const FHitInfo& _hitInfo) override;
};

UCLASS()
class ARPG_HUNTER_API UGimicActionFactory : public UObject 
{
	GENERATED_BODY()
public:
	static TObjectPtr<UGimicAction> CreateGimic(UObject* _worldContext, EGimicType _type);
};