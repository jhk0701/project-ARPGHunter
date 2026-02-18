// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class UItemConfig;
enum class EItemType : uint8;
/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UItem : public UObject
{
	GENERATED_BODY()
private:
	FName ID;
	EItemType Type;
	uint16 Amount{ 1 };

	UPROPERTY()
	TObjectPtr<UItemConfig> Config;

public:
	virtual void Init(const FName& _id, EItemType _type, uint16 _amount, TObjectPtr<UItemConfig> _config);
	virtual bool TryAddAmount(uint16 _amount, uint16& _outRemain);
	bool TrySubAmount(uint16 _amount);

	bool IsFull() const;

	const FName& GetID() const { return ID; }
	EItemType GetType() const { return Type; }
	uint16 GetAmount() const { return Amount; }
	const TObjectPtr<UItemConfig> GetConfig() { return Config; }
};

UCLASS()
class ARPG_HUNTER_API UConsumableItem : public UItem 
{
	GENERATED_BODY()
private:
	// TODO : 아이템 사용 후, 쿨타임 로직
};

UCLASS()
class ARPG_HUNTER_API UEquipmentItem : public UItem
{
	GENERATED_BODY()
private:
	// TODO: 강화 및 인챈트 데이터
public:
	void Init(const FName& _id, EItemType _type, uint16 _amount, TObjectPtr<UItemConfig> _config) override;
};