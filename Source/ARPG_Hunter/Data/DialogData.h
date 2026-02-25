// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogData.generated.h"

/**
 * 
 */
USTRUCT()
struct ARPG_HUNTER_API FDialogData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName NPCName;
	UPROPERTY(EditAnywhere)
	FString Message;
};
