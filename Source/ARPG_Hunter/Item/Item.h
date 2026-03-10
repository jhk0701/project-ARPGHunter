// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class UItemConfig;
enum class EItemType : uint8;
enum class EEquipmentType : uint8;
enum class EWeaponType : uint8;
enum class ECharacterStatType : uint8;

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
	uint8 InventoryIndex{ 0 };
	UPROPERTY()
	TObjectPtr<UItemConfig> Config;

public:
	virtual void Init(const FName& _id, EItemType _type, uint16 _amount, TObjectPtr<UItemConfig> _config);
	virtual bool TryAddAmount(uint16 _amount, uint16& _outRemain);
	bool TrySubAmount(uint16 _amount);
	void SetAmount(uint16 _amount) { Amount = _amount; }
	void SetInventoryIndex(uint8 _index) { InventoryIndex = _index; }

	virtual void GetItemName(FString& _outNameStr) const;
	const FName& GetID() const { return ID; }
	EItemType GetType() const { return Type; }
	uint16 GetAmount() const { return Amount; }
	uint8 GetInventoryIndex() const { return InventoryIndex; }
	TObjectPtr<UItemConfig> GetConfig() const { return Config; }
	
	bool IsFull() const;
};

UCLASS()
class ARPG_HUNTER_API UConsumableItem : public UItem 
{
	GENERATED_BODY()
private:
	int32 QuickSlotIndex{ -1 };

public:
	void Consume(class IEffectable* _target);
	void SetQuickSlotIndex(int32 _idx) { QuickSlotIndex = _idx; }
	int32 GetQuickSlotIndex() const { return QuickSlotIndex; }
};

UCLASS()
class ARPG_HUNTER_API UEquipmentItem : public UItem
{
	GENERATED_BODY()
private:
	// 장착 시, 0 이상 값. EEquipmentType 의 값이 들어올 것
	int32 EquipmentIndex{ -1 }; 
	
	// 강화 수치
	uint8 Grade{ 0 };

public:
	virtual void Init(const FName& _id, EItemType _type, uint16 _amount, TObjectPtr<UItemConfig> _config) override;
	virtual void GetItemName(FString& _outNameStr) const override;
	
	void SetEquipmentIndex(int32 _idx) { EquipmentIndex = _idx; }
	int32 GetEquipmentIndex() const { return EquipmentIndex; }
	bool IsEquiped() const { return EquipmentIndex >= 0; }
	 
	uint8 Upgrade() { return ++Grade; }
	uint8 GetGrade() const { return Grade; }
	void SetGrade(uint8 _newGrade) { Grade = _newGrade; }

	void GetStat(TObjectPtr<class UDataManager> _dataManager, TMap<ECharacterStatType, uint32>& _outEquipmentStat);
	EEquipmentType GetEquipmentType() const;
	EWeaponType GetWeaponType() const;
};