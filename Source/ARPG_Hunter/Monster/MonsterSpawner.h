// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

enum class EMonsterType : uint8;

UCLASS()
class ARPG_HUNTER_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Setting|Monster")
	TMap<EMonsterType, TSubclassOf<class AMonsterBase>> MonsterClass;
	UPROPERTY(EditAnywhere, Category = "Setting|Monster")
	TArray<FName> MonsterIDs;

	UPROPERTY(EditAnywhere, Category = "Setting|Area")
	TObjectPtr<class UBoxComponent> BoxComp;

public:	
	AMonsterSpawner();

protected:
	virtual void BeginPlay() override;

	void SpawnMonster();
};
