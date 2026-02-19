// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerManager.generated.h"

class UInventory;
class UEquipment;
class UQuickSlot;
enum class ECharacterStatType : uint8;
enum class EEquipmentType : uint8;

using FPMAliasMapStat = TMap<ECharacterStatType, uint32>;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrencyChanged, uint32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatValueChanged, const FPMAliasMapStat&, const FPMAliasMapStat&);

USTRUCT()
struct FCurrency 
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint32 Value{ 0 };

	FOnCurrencyChanged OnValueChanged;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UPlayerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPlayerManager();

private:
	UPROPERTY()
	TObjectPtr<class UPlayerConfig> PlayerDefault;
	
	TMap<ECharacterStatType, uint32> Stat;
	FCurrency Gold;
	
	UPROPERTY()
	TObjectPtr<UInventory> Inventory;
	UPROPERTY()
	TObjectPtr<UEquipment> Equipment;
	UPROPERTY()
	TObjectPtr<UQuickSlot> QuickSlot;

	void BroadcastStatChanged();
	void EquipmentStatChanged(const TMap<ECharacterStatType, uint32>& _equipmentStat);

public:
	FOnStatValueChanged OnStatValueChanged;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	const TMap<ECharacterStatType, uint32>& GetStat() const { return Stat; }
	const TMap<ECharacterStatType, uint32>& GetEquipmentStat() const;

	const uint32 GetGold() const { return Gold.Value; }
	void AddGold(uint32 _amount);
	FOnCurrencyChanged& GetGoldChangedEvent() { return Gold.OnValueChanged; }

	const TObjectPtr<UInventory> GetInventory() const { return Inventory; }
	const TObjectPtr<UEquipment> GetEquipment() const { return Equipment; }
	const TObjectPtr<UQuickSlot> GetQuickSlot() const { return QuickSlot; }
	 
	UFUNCTION(BlueprintCallable)
	uint8 AddItem(const FName & _itemID, int32 _amount);
	void ProvideBasicProperty();
	TObjectPtr<USkeletalMesh> GetDefaultMesh(EEquipmentType _type) const;
};
