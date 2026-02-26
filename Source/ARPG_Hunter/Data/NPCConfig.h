// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NPCConfig.generated.h"

USTRUCT()
struct FNPCDialogOption
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FString DialogTitle;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUWPopUp> UIClass;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UNPCConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName NpcID;
	UPROPERTY(EditAnywhere)
	FName FirstDialogID;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMesh> Mesh;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimInstClass;
	UPROPERTY(EditAnywhere)
	TArray<FNPCDialogOption> DialogOptions;
};
