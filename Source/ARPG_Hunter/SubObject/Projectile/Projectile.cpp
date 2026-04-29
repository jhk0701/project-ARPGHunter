// Fill out your copyright notice in the Description page of Project Settings.


#include "SubObject/Projectile/Projectile.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false; // 틱은 수동으로 제어할 것
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddElapsedTime(DeltaTime);

	if (GetElapsedTime() > GetLifeTime())
	{
		Disable();
		return;
	}
	
	AddActorWorldOffset(Speed * DeltaTime * GetFireVector(), true);
}

void AProjectile::Fire(TWeakObjectPtr<AActor> _attacker, const FVector& _vector)
{
	Super::Fire(_attacker, _vector);
	SetActorRotation(GetFireVector().ToOrientationRotator());
}

void AProjectile::Hit(TObjectPtr<AActor> _target, const FHitResult& _result)
{
	Super::Hit(_target, _result);

	Disable();
}
