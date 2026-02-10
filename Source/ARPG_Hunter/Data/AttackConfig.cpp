// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/AttackConfig.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Define/Enum.h"

void UAttackConfig::Act(const FAttackParam& _inParam)
{
	if (_inParam.Subject.IsValid() == false || _inParam.OnHitAction == nullptr)
		return;

	switch (_inParam.DetailType)
	{
	case EAttackDetailType::MELEE_FRONT:		MeleeFront(_inParam); 
		break;
	case EAttackDetailType::MELEE_FRONT_WIDE:	MeleeFrontWide(_inParam);
		break;
	case EAttackDetailType::MELEE_AROUND:		MeleeAround(_inParam);
		break;
	case EAttackDetailType::RANGED_DIRECTIONAL:	RangedDirectional(_inParam);
		break;
	}

}

void UAttackConfig::MeleeFront(const FAttackParam& _inParam)
{
	FVector Fwd = _inParam.Subject->GetActorForwardVector();
	FVector Start = _inParam.Subject->GetActorLocation();
	FVector End = _inParam.Subject->GetActorLocation() + Fwd * _inParam.Range;

	TArray<FHitResult> HitResults;
	bool bIsHit = UKismetSystemLibrary::BoxTraceMulti(
		_inParam.Subject->GetWorld(),
		Start, End,
		FVector(_inParam.Size),
		Fwd.Rotation(),
		UEngineTypes::ConvertToTraceType(_inParam.Channel),
		false,
		{ _inParam.Subject.Get() },
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bIsHit == false)
		return;

	for (const FHitResult& Hit : HitResults)
		_inParam.OnHitAction(Hit);
}

void UAttackConfig::MeleeFrontWide(const FAttackParam& _inParam)
{
	FVector Fwd = _inParam.Subject->GetActorForwardVector();
	FVector Loc = _inParam.Subject->GetActorLocation();

	TArray<FHitResult> HitResults;
	bool bIsHit = UKismetSystemLibrary::BoxTraceMulti(
		_inParam.Subject->GetWorld(),
		Loc, Loc,
		FVector(_inParam.Size, _inParam.Range, _inParam.Size),
		Fwd.Rotation(),
		UEngineTypes::ConvertToTraceType(_inParam.Channel),
		false,
		{ _inParam.Subject.Get() },
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bIsHit == false)
		return;

	for (const FHitResult& Hit : HitResults)
		_inParam.OnHitAction(Hit);
}

void UAttackConfig::MeleeAround(const FAttackParam& _inParam)
{
	FVector Loc = _inParam.Subject->GetActorLocation();

	TArray<FHitResult> HitResults;
	bool bIsHit = UKismetSystemLibrary::SphereTraceMulti(
		_inParam.Subject->GetWorld(),
		Loc, Loc,
		_inParam.Range,
		UEngineTypes::ConvertToTraceType(_inParam.Channel),
		false,
		{ _inParam.Subject.Get() },
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bIsHit == false)
		return;

	for (const FHitResult& Hit : HitResults)
		_inParam.OnHitAction(Hit);

}

void UAttackConfig::RangedDirectional(const FAttackParam& _inParam)
{
}

