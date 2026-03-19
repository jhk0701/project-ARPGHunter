// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Define/InnerTypes.h"
#include "SkillUpgrade.generated.h"

class UActionInstance;
class UEffectData;
struct FActionConnect;
enum class EAttackType : uint8;
enum class EActionEvent : uint8;

struct FAdjustParam
{
public:
	// uint8 Index;
	TArray<TObjectPtr<UActionInstance>>* ActionArray;
	TArray<TMap<EAttackType, FActionConnect>>* Graph;
	TMap<EAttackType, FActionConnect>* GraphStart;
};

/**
 * 
 */
UCLASS(Abstract)
class ARPG_HUNTER_API USkillUpgrade : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FText NameText;
	UPROPERTY(EditAnywhere)
	FString DescString;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Thumbnail;

	virtual void AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param) {};
};

// 잠겨있는 액션 해금
UCLASS()
class ARPG_HUNTER_API USkillNodeUnlockAction : public USkillUpgrade
{
	GENERATED_BODY()
public:
	virtual void AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param) override;
};

// 액션에 있는 이펙트의 효과값 수정
UCLASS()
class ARPG_HUNTER_API USkillNodeModifyEffect : public USkillUpgrade
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UEffectData> TargetEffect;
	UPROPERTY(EditAnywhere)
	uint32 AddictiveValue{ 10 };

	virtual void AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param) override;
};

// 기존 액션에 추가로 이펙트를 부여
UCLASS()
class ARPG_HUNTER_API USkillNodeExtendEffect : public USkillUpgrade
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TMap<EActionEvent, FActionEventEffect> ExtendEffects;

	virtual void AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param) override;
};

// 액션의 공격력 등 수정
UCLASS()
class ARPG_HUNTER_API USkillNodeModifySpec : public USkillUpgrade
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	uint16 AddictiveAttackDamagePer{ 10 };
	UPROPERTY(EditAnywhere)
	uint16 AddictiveStaggerDamage{ 10 };
	UPROPERTY(EditAnywhere)
	uint8 ReduceStaminaUsage{ 1 };

	virtual void AdjustSkillNode(uint8 _targetIdx, FAdjustParam& _param) override;
};