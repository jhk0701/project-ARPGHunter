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
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UWidgetComponent> WidgetComp;

protected:
	virtual void BeginPlay() override;
	virtual void OnDead() override;
	TObjectPtr<UWidgetComponent> GetWidgetComp() { return WidgetComp; }

public:
	virtual void Init(const FMonsterInitParam& _param) override;
	virtual void HitBy(const FHitInfo& _hitInfo) override;

	void KnockBack(const FHitInfo& _hitInfo);
};
