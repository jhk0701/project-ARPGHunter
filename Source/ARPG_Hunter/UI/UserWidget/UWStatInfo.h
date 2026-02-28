// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UWStatInfo.generated.h"

class UTextBlock;
enum class ECharacterStatType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UUWStatInfo : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> NameLabel;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ValueLabel;
public:
	void SetStatName(ECharacterStatType _type);
	
	void SetStatValue(uint32 _value);
	void SetStatValue(const FText& _text);
};

UCLASS()
class ARPG_HUNTER_API UUWStatChangeCompare : public UUWStatInfo
{
	GENERATED_BODY()
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BeforeValueLabel;

public:
	void SetBeforeStatValue(uint32 _value);

};