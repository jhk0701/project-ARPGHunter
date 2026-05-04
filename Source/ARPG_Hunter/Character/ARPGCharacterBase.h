// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Hitable.h"
#include "Interface/Effectable.h"
#include "Interface/AttackNotifyHandler.h"
#include "ARPGCharacterBase.generated.h"

UCLASS(Abstract)
class ARPG_HUNTER_API AARPGCharacterBase : 
	public ACharacter, 
	public IHitable, 
	public IEffectable, 
	public IAttackNotifyHandler
{
	GENERATED_BODY()

public:
	AARPGCharacterBase();

protected:
	/// <summary>
	/// 각 하위 클래스에서 필요한 ActionComponent들을 생성할 것
	/// </summary>
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UActionComponent> ActionComp;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStatComponent> StatComp;

	/// <summary>
	/// 캐릭터가 공격 시, 감지할 적 객체의 충돌 채널
	/// </summary>
	ECollisionChannel EnemyCollisionChannel;

public:
	// Inherited via IEffectable
	TObjectPtr<UStatComponent> GetStatComp() const override { return StatComp; };
	void ApplyEffect(const FApplyEffectParam& _param) override;
	// Inherited via IHitable
	virtual uint32 HitBy(const FHitInfo& _hitInfo) override;

	bool IsDead() const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
		
	void SetEnemyCollisionChannel(ECollisionChannel _channel) { EnemyCollisionChannel = _channel; }
	ECollisionChannel GetEnemyCollisionChannel() const { return EnemyCollisionChannel; }

	template<typename T>
	TObjectPtr<T> GetActionComp() const;

#pragma region Need To Override

public:
	// Inherited via IAttackNotifyHandler
	virtual void HandleAttackNotify(uint8 _opt) override {};

protected:
	virtual void OnDead() {};
	virtual void OnCharacterHit() {};

#pragma endregion
};

template<typename T>
inline TObjectPtr<T> AARPGCharacterBase::GetActionComp() const
{
	return Cast<T>(ActionComp);
}
