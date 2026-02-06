// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monster/RegularMonster/RegularMonster.h"
#include "MeleeMonster.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API AMeleeMonster : public ARegularMonster
{
	GENERATED_BODY()

public:
	AMeleeMonster();

	void HandleAttackNotify(uint8 _opt) override;
};
