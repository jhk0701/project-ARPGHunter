// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ARPG_HUNTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly);
	TObjectPtr<class UBoxComponent> BoxComp;
	UPROPERTY(EditDefaultsOnly);
	TObjectPtr<class UStaticMeshComponent> MeshComp;

	UPROPERTY(EditDefaultsOnly)
	float Speed{100.0f};
	UPROPERTY(EditDefaultsOnly)
	FVector TargetLocation;


public:	
	AProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	void Init(TObjectPtr<class UStaticMesh> _mesh);

	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);
};
