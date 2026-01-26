// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponTypeData.generated.h"

class UAction;
enum class EWeaponType : uint8;

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
	TObjectPtr<class UAction> DodgeAction;
	UPROPERTY(EditAnywhere, Category = "Animation|Attack")
	TObjectPtr<class UActionComboData> AttackCombo;
};