// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "ItemTradeData.generated.h"


USTRUCT()
struct ARPG_HUNTER_API FItemTradeData : public FTableRowBase 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FName ItemID;
	UPROPERTY(EditAnywhere)
	uint32 Price;
	UPROPERTY(EditAnywhere)
	uint32 Amount;
};

USTRUCT()
struct FTradeTable 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> Table;
	UPROPERTY(EditAnywhere)
	float Weight{1.0f};
};

UCLASS()
class ARPG_HUNTER_API UItemTradeConfig : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FTradeTable> TradeTables;

	TObjectPtr<UDataTable> GetRandomTable() const;
};
