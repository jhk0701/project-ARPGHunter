// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ActionComboData.generated.h"


class UAction;
enum class EAttackType : uint8;

USTRUCT(BlueprintType)
struct FActionNode
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EAttackType, uint8> Edge;
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
	TArray<FActionNode> Graph;

	UPROPERTY(EditAnywhere)
	TMap<EAttackType, uint8> Start;
};
