// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "Interface/GimicHandler.h"
#include "BossMonster.generated.h"

enum class EMonsterAttackType : uint8;
enum class EGimicType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ABossMonster : public AMonsterBase, public IGimicHandler
{
	GENERATED_BODY()
private:
	// Player HUD에 보스 체력바 출력
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UUserWidget> StatusBarClass;
	UPROPERTY(VisibleAnywhere, Category = "UI")
	TObjectPtr<UUserWidget> StatusBar;
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector2D  DamageFontYRange{0.0f, 50.0f};

	FTimerHandle GroggyRecoverTimer;

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void OnDead() override;
	void ShowDamageUI(bool _bIsCritical, uint32 _damage) override;

public:
	ABossMonster();

	void Init(const FMonsterInitParam& _param) override;
	uint32 HitBy(const FHitInfo& _hitInfo) override;

	bool CanUseGimic() override;
	void HandleGimicNotify(EGimicType _type, uint16 _gimicValue) override;

	void OnAlertStateChanged(EMonsterAlertState _prevState, EMonsterAlertState _nextState) override;
	virtual void TriggerReactForPlayerAction(uint8 _actionType) override;
};
