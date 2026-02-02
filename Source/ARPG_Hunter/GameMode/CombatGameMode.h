// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/ARPGGameMode.h"
#include "CombatGameMode.generated.h"

struct FStageData;
struct FSection;
enum class EMonsterType : uint8;
struct FMonsterSpawn;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ACombatGameMode : public AARPGGameMode
{
	GENERATED_BODY()

private:
	FStageData* StageData;

	UPROPERTY(EditDefaultsOnly, Category = "Setting|Monster")
	TMap<EMonsterType, TSubclassOf<class AMonsterBase>> MonsterClass;

public:
	ACombatGameMode();
	const FSection& GetSection(uint8 _idx) const;

	uint8 SpawnMonsterOnSection(uint8 _sectionID, const FVector& _point, const FVector& _areaSize);

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

private:
	void SetObjectPool();

};
