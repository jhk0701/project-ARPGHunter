// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubObject/SubObject.h"
#include "Projectile.generated.h"

UCLASS()
class ARPG_HUNTER_API AProjectile : public ASubObject
{
	GENERATED_BODY()

public:
	AProjectile();

private:
	UPROPERTY(EditDefaultsOnly)
	float Speed{ 100.0f };

public:
	virtual void Tick(float DeltaTime) override;
	void Fire(TWeakObjectPtr<AActor> _attacker, const FVector& _vector) override;
	
	void Hit(TObjectPtr<AActor> _target, const FHitResult& _result) override;
};
