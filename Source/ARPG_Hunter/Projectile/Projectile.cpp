// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	BoxComp->SetCollisionProfileName(FName(TEXT("OverlapPlayer")));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnBeginOverlap);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldOffset(Speed * DeltaTime * Direction);
}

void AProjectile::Init()
{
	// MeshComp->SetStaticMesh(_mesh);
}

void AProjectile::Fire(TWeakObjectPtr<AActor> _target)
{
	Target = _target;
	Direction = Target.Get()->GetActorLocation() - GetActorLocation();
	Direction.Normalize();

	SetActorRotation(Direction.ToOrientationRotator());
}

void AProjectile::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectile::Hit()
{
}

