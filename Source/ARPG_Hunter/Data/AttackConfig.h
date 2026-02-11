// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackConfig.generated.h"

enum class EAttackDetailType : uint8;

struct FAttackParam
{
	TWeakObjectPtr<AActor> Subject;
	TWeakObjectPtr<AActor> Target;
	EAttackDetailType DetailType;
	ECollisionChannel Channel;
	float Size;
	float Range;
	TFunction<void(FHitResult&)> OnHitAction;
};

UCLASS()
class ARPG_HUNTER_API UAttackConfig : public UObject
{
	GENERATED_BODY()
public:
	static void Act(const FAttackParam& _inParam);
private:
	static void MeleeFront(const FAttackParam& _inParam);
	static void MeleeFrontWide(const FAttackParam& _inParam);
	static void MeleeAround(const FAttackParam& _inParam);
	static void Ranged(const FAttackParam& _inParam);
};