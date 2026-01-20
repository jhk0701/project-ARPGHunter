// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponTypeData.generated.h"

enum class EWeaponType : uint8;

/*
* 플레이어 액션 구조체
*/
USTRUCT()
struct FAction
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint16 StaminaUsage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Montage;
};

/*
* Action
*/
USTRUCT()
struct FActionList
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FAction StartAction;
	UPROPERTY(EditAnywhere)
	TArray<FAction> LinkedAction;
};

/**
 * 무기별 정보용 데이터 테이블 행
 */
USTRUCT()
struct ARPG_HUNTER_API FWeaponTypeData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EWeaponType Type;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TObjectPtr<UAnimMontage> HitMontage;
	UPROPERTY(EditAnywhere, Category = "Animation")
	FAction DodgeAction;
	UPROPERTY(EditAnywhere, Category = "Animation|Attack")
	TArray<FActionList> AttackAction;
};