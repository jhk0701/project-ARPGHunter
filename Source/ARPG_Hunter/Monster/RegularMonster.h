// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/MonsterBase.h"
#include "RegularMonster.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ARegularMonster : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	ARegularMonster();

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	FVector2D DamageFontYRange{ -50.0f, 50.0f };

	UPROPERTY(EditAnywhere, Category = "UI")
	TObjectPtr<class UWidgetComponent> WidgetComp;

protected:
	void BeginPlay() override;
	void ShowDamageUI(bool _bIsCritical, uint32 _damage) override;

	TObjectPtr<UWidgetComponent> GetWidgetComp() { return WidgetComp; }

public:
	void Init(const FMonsterInitParam& _param) override;
	uint32 HitBy(const FHitInfo& _hitInfo) override;

	void OnAlertStateChanged(EMonsterAlertState _prevState, EMonsterAlertState _nextState) override;
	void TriggerReactForPlayerAction(uint8 _actionType) override;

	void KnockBack(const FHitInfo& _hitInfo);
};
