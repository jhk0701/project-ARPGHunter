// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Inventory.generated.h"

class UItem;
USTRUCT()
struct FIndexArray 
{
	GENERATED_BODY()
public:
	TArray<uint8> Arr;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UInventory : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UItem>> Container;
	UPROPERTY(VisibleAnywhere)
	TMap<UClass*, FIndexArray> ItemIndex; // ItemConfig - Container Index Array

	TObjectPtr<UItem> CreateInstance(TObjectPtr<UObject> _worldContext, const FName& _id, uint16 _amount);

public:
	void Init(uint8 _size = 80);

	bool TryAddItem(TObjectPtr<UObject> _worldContext, const FName& _id, uint16 _amount, uint8& _outIdx);
	bool TrySubItem(uint8 _idx);
	bool TryFindItem(const FName& _id, uint8& _outIdx);
};
