// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GimicHandler.generated.h"

enum class EGimicType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGimicHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARPG_HUNTER_API IGimicHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CanUseGimic() = 0;
	virtual void HandleGimicNotify(EGimicType _type, uint16 _gimicValue) = 0;
};
