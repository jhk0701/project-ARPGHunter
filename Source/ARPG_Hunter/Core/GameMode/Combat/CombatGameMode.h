// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameMode/ARPGGameMode.h"
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
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGameEnd, bool, const FStageData*);

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
	UPROPERTY()
	TMap<FName, TObjectPtr<class APlayerStart>> MapPlayerStart; // Tag - PlayerStart

public:
	ACombatGameMode();

private:
	void RegisterObjectPool();
	void ReleaseMonster(TObjectPtr<class AMonsterBase> _target);
	void ReleaseSubObject(TObjectPtr<AActor> _target);

	void GameClear();
	void GameFail();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void SetStageData(const FName& _stageID);

public:
	// 전투 관련 계산식
	static uint32 CalculateAttack(uint32 _baseVal, uint32 _AddictivePercent);
	static bool CalculateCritical(uint32 _critPer, uint32 _critDmg, uint32& _outDamage);
	static uint32 CalculateDefense(uint32 _damage, uint32 _defense);

	TMap<EStageEvent, FStageEvent> StageEvent;
	FOnGameEnd OnGameEnd;

	void PublishEvent(EStageEvent _event, const FStageEventContext& _context) { StageEvent[_event].Broadcast(_context); }

	// PlayerStart 중 하나에서 스폰하기 위해 Blueprint Native 함수 오버라이드
	virtual AActor* ChoosePlayerStart_Implementation(AController* _player) override;
	uint8 SpawnMonsterOnSection(uint8 _sectionID, const TArray<AActor*>& _points);
	TObjectPtr<class UCutSceneAssetData> GetCutSceneAsset(uint8 _idx) const;
};
