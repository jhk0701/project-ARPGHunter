// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMode/ARPGGameMode.h"
#include "CombatGameMode.generated.h"

struct FStageData;
struct FSection;
enum class EMonsterType : uint8;

enum class EStageEvent : uint8
{
	HUNT,
	SECTION_CLEAR,
	PLAYER_DEAD,

	END
};

struct FStageEventContext
{
	uint8 SectionIndex;
	TObjectPtr<UObject> Target;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FStageEvent, const FStageEventContext&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameEnd, bool);

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

	FOnGameEnd OnGameEnd;
	TMap<EStageEvent, FStageEvent> StageEvent;
	void PublishEvent(EStageEvent _event, const FStageEventContext& _context) { StageEvent[_event].Broadcast(_context); }

	uint8 SpawnMonsterOnSection(uint8 _sectionID, const FVector& _point, const FVector& _areaSize);

	void GameFail();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

private:
	void SetMonsterPool();
	void ReleaseMonster(TObjectPtr<class AMonsterBase> _target);
	void GameClear();

};
