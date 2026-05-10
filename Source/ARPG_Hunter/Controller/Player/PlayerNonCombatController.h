// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Controller/Player/PlayerCharacterController.h"
#include "PlayerNonCombatController.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API APlayerNonCombatController : public APlayerCharacterController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* _pawn) override;

	virtual void ShortCut(EShortCutType _key) override;
};
