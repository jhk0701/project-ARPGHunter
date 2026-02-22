// Fill out your copyright notice in the Description page of Project Settings.


#include "Misc/RandomMesh.h"
#include "Components/BoxComponent.h"

// Sets default values
ARandomMesh::ARandomMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	SetRootComponent(BoxComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(BoxComp);
}

void ARandomMesh::SetRandomMesh()
{
	if (MeshList.Num() == 0)
		return;

	int32 r = FMath::RandRange(0, MeshList.Num() - 1);
	MeshComp->SetStaticMesh(MeshList[r]);

	FVector Extent = MeshList[r]->GetBounds().BoxExtent;
	BoxComp->SetBoxExtent(Extent);
	MeshComp->SetRelativeLocation(FVector(0, 0, -Extent.Z));

	if (bUpdateLocation)
	{
		FVector vecBoxSize = BoxComp->GetScaledBoxExtent();
		FVector Loc = GetActorLocation();
		Loc.Z -= vecBoxSize.Z;

		SetActorLocation(FVector(Loc.X, Loc.Y, Loc.Z + vecBoxSize.Z));
	}
}

void ARandomMesh::SetRandomTransform()
{
	FVector Loc = GetActorLocation() + FVector(
		FMath::FRandRange(MinLocationOffset.X, MaxLocationOffset.X),
		FMath::FRandRange(MinLocationOffset.Y, MaxLocationOffset.Y),
		FMath::FRandRange(MinLocationOffset.Z, MaxLocationOffset.Z)
	);

	FRotator Rot = FRotator(
		FMath::FRandRange(MinRotation.Pitch, MaxRotation.Pitch),
		FMath::FRandRange(MinRotation.Yaw, MaxRotation.Yaw),
		FMath::FRandRange(MinRotation.Roll, MaxRotation.Roll)
	);

	FVector Scale = FVector(
		FMath::FRandRange(MinScale.X, MaxScale.X),
		FMath::FRandRange(MinScale.Y, MaxScale.Y),
		FMath::FRandRange(MinScale.Z, MaxScale.Z)
	);

	FTransform Transform;
	Transform.SetLocation(Loc);
	Transform.SetRotation(Rot.Quaternion());
	Transform.SetScale3D(Scale);
	SetActorTransform(Transform);
}

