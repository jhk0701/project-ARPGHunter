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
	FName StageID; // 간단한 Stage ID 보관이라 GameInstance에서 바로 호출

protected:
	virtual void OnStart() override;

public:
	void SetStageID(const FName& _id) { StageID = _id; }
	const FName& GetStageID() const { return StageID; }
};
