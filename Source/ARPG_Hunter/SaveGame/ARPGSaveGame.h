// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ARPGSaveGame.generated.h"

UCLASS()
class ARPG_HUNTER_API UARPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static FString SlotName; // 간편하게 이름 찾기 용도
};
