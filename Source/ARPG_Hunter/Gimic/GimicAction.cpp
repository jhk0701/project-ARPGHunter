// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimic/GimicAction.h"
#include "Define/Enum.h"
#include "Interface/GimicHandler.h"
#include "Interface/Hitable.h"


void UGimicAction::Proceed(float _deltaTime)
{
	ElapsedTime += _deltaTime;

	if (IsTimeOut() == false || Subject.IsValid() == false)
		return;

	if (IGimicHandler* GimicHandler = Cast<IGimicHandler>(Subject.Get()))
		GimicHandler->StopGimic(Type);
}


UCounterGimic::UCounterGimic()
{
	SetType(EGimicType::COUNTER);
	EndureCount = 1;
}

void UCounterGimic::Interrupt(const FHitInfo& _hitInfo)
{
	// 맞은 범위가 정면이며, 스매시 이상 공격이었을 경우, 방해처리
	Super::Interrupt(_hitInfo);

	TWeakObjectPtr<AActor> GimicSubject = GetSubject();
	if (_hitInfo.Attacker.IsValid() == false || 
		GimicSubject.IsValid() == false)
		return;

	FVector AttackFwd = _hitInfo.Attacker->GetActorForwardVector();
	FVector SubjectFwd = GimicSubject->GetActorForwardVector();

	double Dot = FVector::DotProduct(AttackFwd, SubjectFwd);
	// 정면 45도 기준
	if (Dot < 0 && FMath::RadiansToDegrees(FMath::Acos(Dot)) < 45.0f * 0.5f)
		EndureCount--;

	if (EndureCount > 0)
		return;
	
	if (IGimicHandler* GimicHandler = Cast<IGimicHandler>(GimicSubject.Get()))
		GimicHandler->StopGimic(GetType());
}


UStaggerGimic::UStaggerGimic()
{
	SetType(EGimicType::STAGGER);
}


TObjectPtr<UGimicAction> UGimicActionFactory::CreateGimic(TWeakObjectPtr<AActor> _subject, EGimicType _type)
{
	if (_subject.IsValid() == false)
		return nullptr;

	TObjectPtr<UGimicAction> Instance;

	switch (_type)
	{
	case EGimicType::COUNTER:
		Instance = NewObject<UCounterGimic>(_subject.Get()->GetWorld());
	case EGimicType::STAGGER:
		Instance = NewObject<UStaggerGimic>(_subject.Get()->GetWorld());
	default:
		return nullptr;
	}

	Instance->SetSubject(_subject.Get());
	return Instance;
}