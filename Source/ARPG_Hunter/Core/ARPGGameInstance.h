// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ARPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UARPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	FName StageID;

public:
	UARPGGameInstance();

	void SetStageID(const FName& _id) { StageID = _id; }
	const FName& GetStageID() const { return StageID; }
};
