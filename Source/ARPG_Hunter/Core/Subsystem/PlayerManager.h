// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interface/SaveLoadHandler.h"
#include "PlayerManager.generated.h"

class UInventory;
class UEquipment;
class UQuickSlot;
class USkillDevelop;
enum class ECharacterStatType : uint8;
enum class EEquipmentType : uint8;

using FPMAliasMapStat = TMap<ECharacterStatType, uint32>; // 델리게이트 매개변수 선언용

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrencyChanged, uint32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatValueChanged, const FPMAliasMapStat&, const FPMAliasMapStat&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnExpChanged, uint16, int32, int32);

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
class ARPG_HUNTER_API UPlayerManager : public UGameInstanceSubsystem, public ISaveLoadHandler
{
	GENERATED_BODY()

public:
	UPlayerManager();

private:
	FString PlayerName;
	const FName NAME_EXP{ FName(TEXT("Exp")) };
	const FName NAME_SkillPoint{ FName(TEXT("SkillPoint")) };

	UPROPERTY()
	TObjectPtr<class UPlayerConfig> PlayerDefault;
	uint16 Level{ 0 };
	int32 RequiredExp{ 0 };
	int32 Exp{ 0 };
	TMap<ECharacterStatType, uint32> Stat;
	FCurrency Gold;
	
	UPROPERTY()
	TObjectPtr<UInventory> Inventory;
	UPROPERTY()
	TObjectPtr<UEquipment> Equipment;
	UPROPERTY()
	TObjectPtr<UQuickSlot> QuickSlot;
	UPROPERTY()
	TObjectPtr<USkillDevelop> SkillDevelop;

	void EquipmentStatChanged(const TMap<ECharacterStatType, uint32>& _equipmentStat);
	void QuickSlotItemUsed(uint8 _quickSlotIdx, uint8 _inventoryIdx);

	void ProvideBasicProperty();

public:
	TWeakObjectPtr<UInventory> GetInventory() const;
	TWeakObjectPtr<UEquipment> GetEquipment() const;
	TWeakObjectPtr<UQuickSlot> GetQuickSlot() const;
	TWeakObjectPtr<USkillDevelop> GetSkillDevelop() const;

	FOnStatValueChanged OnStatValueChanged;
	FOnExpChanged OnExpChanged;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void PostInit();

	void CreateNewPlayer(const FString& _playerName);

	const FString& GetPlayerName() const { return PlayerName; }
	const TMap<ECharacterStatType, uint32>& GetStat() const { return Stat; }
	const TMap<ECharacterStatType, uint32>& GetEquipmentStat() const;
	TWeakObjectPtr<class UWeaponConfig> GetWeaponConfig() const;

	const uint32 GetGold() const { return Gold.Value; }
	void AddGold(uint32 _amount);
	bool TrySubGold(uint32 _amount);
	FOnCurrencyChanged& GetGoldChangedEvent() { return Gold.OnValueChanged; }
 
	TObjectPtr<USkeletalMesh> GetDefaultMesh(EEquipmentType _type) const;
	TWeakObjectPtr<class UConsumableItem> GetQuickSlotItem(uint8 _idx) const;
	void UseQuickSlotItem(uint8 _index, class IEffectable* _target);
	
	uint8 AddItem(const FName& _itemID, uint8 _amount);
	void AddExp(uint32 _exp);
	void LevelUp();
	void AdjustStatByLevel();
	uint16 GetLevel() const { return Level; }
	int32 GetExp() const { return Exp; }
	int32 GetRequiredExp() const { return RequiredExp; }

	// Inherited via ISaveLoadHandler
	virtual void WriteSaveData(UARPGSaveGame* _savegame) override;
	virtual void ReadSaveData(UARPGSaveGame* _savegame) override;

	UFUNCTION(BlueprintCallable)
	void exeAddExp(int32 _exp) { AddExp(_exp); };
	UFUNCTION(BlueprintCallable)
	void exeAddItem(const FName& _itemID, int32 _amount) { AddItem(_itemID, _amount); };
};
