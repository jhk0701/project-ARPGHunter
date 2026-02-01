// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageSection.generated.h"

enum class EMonsterType : uint8;
struct FMonsterSpawn;

UCLASS()
class ARPG_HUNTER_API AStageSection : public AActor
{
	GENERATED_BODY()
	
private:
	enum EState : uint8
	{
		READY,
		IN_PROGRESS,
		CLEARED
	};

	UPROPERTY(EditAnywhere, Category = "Setting")
	uint8 Index{0};

	EState State{ EState::READY };

	UPROPERTY(VisibleAnywhere, Category = "Setting")
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "Setting|Monster")
	TMap<EMonsterType, TSubclassOf<class AMonsterBase>> MonsterClass;

public:	
	// Sets default values for this actor's properties
	AStageSection();
	void SpawnMonster(const FMonsterSpawn& _spawnData);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	void PostInitializeComponents() override;
	void BeginPlay() override;
};
