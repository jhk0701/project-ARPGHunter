// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerManager.generated.h"

class UInventory;
enum class ECharacterStatType : uint8;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrencyChanged, uint32);

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
	
private:
	UPROPERTY(EditAnywhere)
	TMap<ECharacterStatType, uint32> Stat;

	UPROPERTY(EditAnywhere)
	FCurrency Gold;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInventory> Inventory;

public:
	UPlayerManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	const TMap<ECharacterStatType, uint32>& GetPlayerStat() const { return Stat; }

	const uint32 GetGold() const { return Gold.Value; }
	void AddGold(uint32 _amount);
	FOnCurrencyChanged& GetGoldChangedEvent() { return Gold.OnValueChanged; }
	
	void AddItem(const FName & _itemID, uint16 _amount);

	UFUNCTION(BlueprintCallable)
	void TestAddItem(const FName& _itemID, int _amount);
};
