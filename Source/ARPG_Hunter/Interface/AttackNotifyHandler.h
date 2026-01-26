// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackNotifyHandler.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttackNotifyHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_HUNTER_API IAttackNotifyHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HandleAttackNotify(uint8 _opt) = 0;
};
