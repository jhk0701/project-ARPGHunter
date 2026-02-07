// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/Projectile.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Interface/Hitable.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false; // 틱은 수동으로 제어할 것

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
	ElapsedTime += DeltaTime;

	if (ElapsedTime > LifeTime)
	{
		Disable();
		return;
	}
	
	AddActorWorldOffset(Speed * DeltaTime * Direction, true, &HitResult);
	
}

void AProjectile::Init()
{
	ElapsedTime = 0.f;
	// MeshComp->SetStaticMesh(_mesh);
}

void AProjectile::Fire(TWeakObjectPtr<AActor> _attacker, TWeakObjectPtr<AActor> _target)
{
	Attacker = _attacker;
	Target = _target;
	Direction = Target.Get()->GetActorLocation() - GetActorLocation();
	Direction.Normalize();
	
	SetActorRotation(Direction.ToOrientationRotator());

	SetActorTickEnabled(true); // 틱 시작
}

void AProjectile::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (nullptr == OtherActor)
		return;

	Hit(OtherActor);
}

void AProjectile::Hit(TObjectPtr<AActor> _target)
{
	if (IHitable* Hitable = Cast<IHitable>(_target))
	{
		FHitInfo HitInfo;
		HitInfo.Damage = Damage;
		HitInfo.Attacker = Attacker;
		HitInfo.HitResult = &HitResult;
		Hitable->HitBy(HitInfo);
	}

	Disable();
}

void AProjectile::Disable()
{
	OnDisable.ExecuteIfBound(this);
	SetActorTickEnabled(false); // 틱 비활성화
}

