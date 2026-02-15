// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class UItemConfig;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UItem : public UObject
{
	GENERATED_BODY()
private:
	uint16 Amount{ 1 };
	UPROPERTY()
	TObjectPtr<UItemConfig> Config;

public:
	void Init(uint16 _amount, TObjectPtr<UItemConfig> _config);

	bool TryAddAmount(uint16 _amount, uint16& _outRemain);
	bool TrySubAmount(uint16 _amount);

	bool IsFull() const;
	uint16 GetAmount() const { return Amount; }
};

UCLASS()
class ARPG_HUNTER_API UEquipableItem : public UItem
{
	GENERATED_BODY()
private:
	// TODO: 강화 및 인챈트 정보 추가
public:
};