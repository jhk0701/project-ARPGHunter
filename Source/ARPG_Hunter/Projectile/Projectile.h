// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

DECLARE_DELEGATE_OneParam(FOnProjectileDisable, TObjectPtr<AActor>);

UCLASS()
class ARPG_HUNTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly);
	TObjectPtr<class UBoxComponent> BoxComp;
	UPROPERTY(EditDefaultsOnly);
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor> Attacker;
	UPROPERTY(VisibleAnywhere)
	TWeakObjectPtr<AActor> Target;

	UPROPERTY(VisibleAnywhere)
	FVector Direction;
	FHitResult HitResult;

	UPROPERTY(VisibleAnywhere)
	float LifeTime{ 3.0f };
	UPROPERTY(VisibleAnywhere)
	float ElapsedTime{ 0.0f };
	UPROPERTY(EditDefaultsOnly)
	float Speed{ 100.0f };
	UPROPERTY(EditDefaultsOnly)
	float Damage{ 20.0f };

public:	
	AProjectile();
	FOnProjectileDisable OnDisable;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	void Init();
	void Fire(TWeakObjectPtr<AActor> _attacker, TWeakObjectPtr<AActor> _target);

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	void Hit(TObjectPtr<AActor> _target);
	void Disable();
};
