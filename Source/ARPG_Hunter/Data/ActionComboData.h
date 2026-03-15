// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionComboData.generated.h"


class UAction;
enum class EAttackType : uint8;

USTRUCT()
struct FConnectInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint8 Index;
	UPROPERTY(EditAnywhere)
	bool bIsOptional; // 기본 연결 표시
};

USTRUCT()
struct FActionConnection
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EAttackType, FConnectInfo> Edge;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UActionComboData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UAction>> AttackAcionArray;

	UPROPERTY(EditAnywhere)
	FActionConnection Start;

	UPROPERTY(EditAnywhere)
	TArray<FActionConnection> Graph;
};
