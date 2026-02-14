// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class UAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActionComponent();

private:
	TObjectPtr<UAnimInstance> OwnerAnimInstance{ nullptr };
	TObjectPtr<UAction> CurrentAction{ nullptr };

	UPROPERTY(VisibleAnywhere);
	TObjectPtr<USkeletalMeshComponent> FirePointComp;
	UPROPERTY(EditAnywhere);
	FName FirePointSocketName{TEXT("socket_firePoint")};
	
protected:
	void ActivateActionEffect(const TArray<TObjectPtr<class UEffectData>>& _effectArray, TObjectPtr<AActor> _target);

	void SetCurrentAction(TObjectPtr<UAction> _action) { CurrentAction = _action; }

	TObjectPtr<UAnimMontage> GetCurrentMontage();
	TObjectPtr<UAnimInstance> GetAnimInstance() { return OwnerAnimInstance; }

	bool Trace(uint8 _opt, ECollisionChannel _traceChannel, TArray<FHitResult>& _outResults);
	void Deploy(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction);

public:	
	void Init(TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp);
	virtual void Clear() {};

	// 실질적인 공격 수행
	void ProcessAttack(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction);

	TObjectPtr<UAction> GetCurrentAction() { return CurrentAction; }
	
};
