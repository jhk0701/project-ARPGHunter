// Fill out your copyright notice in the Description page of Project Settings.


#include "SubObject/SubObject.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Interface/Hitable.h"
#include "Data/SubObjectConfig.h"

ASubObject::ASubObject()
{
	PrimaryActorTick.bCanEverTick = false; // 기본적으로는 false로 둘 것이나, 필요하면 자식 클래스에서 true로 바꿔쓸 것

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	BoxComp->SetCollisionProfileName(FName(TEXT("OverlapPlayer")));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ASubObject::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ASubObject::OnBeginOverlap);
}

void ASubObject::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (nullptr == OtherActor)
		return;

	Hit(OtherActor, SweepResult);
}

void ASubObject::Init(TObjectPtr<USubObjectConfig> _config, TFunction<void(TArray<FHitResult>&)> _onHitAction)
{
	Config = _config;
	ElapsedTime = 0.f;
	MeshComp->SetStaticMesh(Config->Mesh);
	BoxComp->SetBoxExtent(Config->Mesh->GetBoundingBox().GetExtent());

	OnHit = _onHitAction;
}

void ASubObject::Fire(TWeakObjectPtr<AActor> _attacker, const FVector& _vector)
{
	Attacker = _attacker;
	FireVector = _vector;
	SetActorTickEnabled(true); // 틱 시작
}

void ASubObject::Disable()
{
	OnDisable.ExecuteIfBound(this);
	SetActorTickEnabled(false); // 틱 비활성화
}

void ASubObject::Hit(TObjectPtr<AActor> _target, const FHitResult& _result)
{
	if (OnHit) 
	{
		TArray<FHitResult> ArrResult;
		ArrResult.Add(_result);

		OnHit(ArrResult);
	}
}
