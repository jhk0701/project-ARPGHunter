// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageSection.generated.h"

enum class EMonsterType : uint8;
struct FMonsterSpawn;
enum class EStageEvent : uint8;
struct FStageEventContext;

UCLASS()
class ARPG_HUNTER_API AStageSection : public AActor
{
	GENERATED_BODY()

public:
	enum EState : uint8
	{
		READY,
		IN_PROGRESS,
		CLEARED
	};

	AStageSection();

private:
	UPROPERTY(VisibleAnywhere, Category = "Section")
	TObjectPtr<class UBoxComponent> BoxComp;
	UPROPERTY(EditAnywhere, Category = "Section|Setting")
	uint8 Index{ 0 };

	EState State{ EState::READY };
	FDelegateHandle EventHandle;

	UPROPERTY(VisibleAnywhere, Category = "Section")
	uint8 SpawnedCount{ 0 };

	UPROPERTY(VisibleAnywhere, Category = "Section")
	TArray<AActor*> SpawnPoints;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	void SetState(EState _state) { State = _state; }

public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void OnMonsterDead(const FStageEventContext& _context);

	virtual void BeginSection();
	virtual void EndSection();
	void SpawnMonster();
	bool IsCleared() const { return State == EState::CLEARED; }
};
