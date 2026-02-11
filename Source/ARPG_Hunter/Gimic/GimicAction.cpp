// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimic/GimicAction.h"
#include "Define/Enum.h"
#include "Interface/GimicHandler.h"
#include "Interface/Hitable.h"


void UGimicAction::Proceed(float _deltaTime)
{
	if (IsTimeOut() || Subject.IsValid() == false)
		return;

	ElapsedTime += _deltaTime;

	if (IsTimeOut())
	{
		if (IGimicHandler* GimicHandler = Cast<IGimicHandler>(Subject.Get()))
			GimicHandler->CompleteGimic();
	}
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
	if (Dot < 0 && FMath::RadiansToDegrees(FMath::Acos(Dot)) < 90.0f * 0.5f)
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

TObjectPtr<UGimicAction> UGimicActionFactory::CreateGimic(UObject* _worldContext, EGimicType _type)
{
	if (nullptr == _worldContext)
		return nullptr;

	TObjectPtr<UGimicAction> Instance = nullptr;

	switch (_type)
	{
	case EGimicType::COUNTER:
		Instance = NewObject<UCounterGimic>(_worldContext);
	case EGimicType::STAGGER:
		Instance = NewObject<UStaggerGimic>(_worldContext);
	}

	return Instance;
}