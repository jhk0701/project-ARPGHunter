// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

enum class EAttackDetailType : uint8;

struct FTraceParam 
{
	EAttackDetailType DetailType;
	float Range;
	FVector Size;
};

struct FSubObjectDeployParam 
{
	EAttackDetailType DetailType;
	UClass* SubObjectClass;
	TObjectPtr<class USubObjectConfig> SubObjectConfig;
};


UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARPG_HUNTER_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UActionComponent();

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowTrace{false};

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<UAnimInstance> OwnerAnimInstance{ nullptr };
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<USkeletalMeshComponent> FirePointComp{ nullptr };
	UPROPERTY(EditAnywhere)
	FName FirePointSocketName{TEXT("socket_firePoint")};

public:
	// 실질적인 공격 수행
	virtual bool ProcessAttack(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction, TWeakObjectPtr<AActor> _target = nullptr);
		
protected:
	bool Trace(const FTraceParam& _param, ECollisionChannel _traceChannel, TArray<FHitResult>& _outResults);
	void DeploySubObject(const FSubObjectDeployParam& _param, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction, TWeakObjectPtr<AActor> _target = nullptr);
	void SpawnHitVFX(class UNiagaraSystem* _vfx, const FVector& _location, float _roll, float _size);
	
	void SetAnimInstance(TWeakObjectPtr<UAnimInstance> _instance) { OwnerAnimInstance = _instance; }
	void SetFirePointComp(TWeakObjectPtr<USkeletalMeshComponent> _comp) { FirePointComp = _comp; }
	TWeakObjectPtr<UAnimInstance> GetAnimInstance() { return OwnerAnimInstance; }
	TWeakObjectPtr<USkeletalMeshComponent> GetFirePointComp() { return FirePointComp; }

	void ActivateActionEffect(TObjectPtr<AActor> _target, const TArray<TObjectPtr<class UEffectData>>& _effectArray);

	//하위에서 구현할 메서드
#pragma region Need To Override

public:
	virtual void Clear() {};
	virtual bool IsValid() const { return false; }

protected:
	virtual TWeakObjectPtr<class UAction> GetCurrentAction() const { return nullptr; }
	virtual void PostProcessAttack(uint8 _opt, const TArray<FHitResult>& _inHitResults) {}

#pragma endregion
};
