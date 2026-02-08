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
	
	AddActorWorldOffset(Speed * DeltaTime * Direction, true, GetHitResult());
}

void AProjectile::Fire(TWeakObjectPtr<AActor> _attacker, TWeakObjectPtr<AActor> _target)
{
	Super::Fire(_attacker, _target);

	Direction = _target.Get()->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	
	SetActorRotation(Direction.ToOrientationRotator());
}
