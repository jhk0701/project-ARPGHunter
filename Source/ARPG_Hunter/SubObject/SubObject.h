// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SubObject.generated.h"

class USubObjectConfig;

DECLARE_DELEGATE_OneParam(FOnDisable, TObjectPtr<AActor>);

UCLASS(Abstract)
class ARPG_HUNTER_API ASubObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ASubObject();
	FOnDisable OnDisable;

private:
	UPROPERTY(EditDefaultsOnly);
	TObjectPtr<class UBoxComponent> BoxComp;
	UPROPERTY(EditDefaultsOnly);
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere);
	TObjectPtr<USubObjectConfig> Config;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor> Attacker;
	UPROPERTY(VisibleAnywhere)
	FVector FireVector; // 투사체의 방향 혹은 떨어져야하는 위치 등 용도

	UPROPERTY(VisibleAnywhere)
	float LifeTime{ 3.0f };
	UPROPERTY(VisibleAnywhere)
	float ElapsedTime{ 0.0f };
	UPROPERTY(EditDefaultsOnly)
	float Damage{ 20.0f };

	TFunction<void(TArray<FHitResult>&)> OnHit;

public:
	virtual void Init(TObjectPtr<USubObjectConfig> _config, TFunction<void(TArray<FHitResult>&)> _onHitAction);
	virtual void Fire(TWeakObjectPtr<AActor> _attacker, const FVector& _vector);
	virtual void Disable();

	UFUNCTION()
	virtual void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
	virtual void Hit(TObjectPtr<AActor> _target, const FHitResult& _result);
	
	void SetElapsedTime(float _time) { ElapsedTime = _time; }
	void AddElapsedTime(float _time) { ElapsedTime += _time; }

	float GetElapsedTime() const { return ElapsedTime; }
	float GetLifeTime() const { return LifeTime; }
	const FVector& GetFireVector() { return FireVector; }

};
