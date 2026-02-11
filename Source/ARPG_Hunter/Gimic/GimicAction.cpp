// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimic/GimicAction.h"
#include "Define/Enum.h"
#include "Interface/GimicHandler.h"
#include "Interface/Hitable.h"
#include "Data/MonsterData.h"

void UGimicAction::Start(TWeakObjectPtr<AActor> _subject, const FGimicParam& _param)
{
	Subject = _subject;
	LifeTime = _param.LifeTime;
};

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

void UCounterGimic::Start(TWeakObjectPtr<AActor> _subject, const FGimicParam& _param)
{
	Super::Start(_subject, _param);
	EndureCount = _param.OptionValue;
}

void UCounterGimic::Interrupt(const FHitInfo& _hitInfo)
{
	// 맞은 범위가 정면이며, 스매시 이상 공격이었을 경우, 방해처리
	Super::Interrupt(_hitInfo);

	if (_hitInfo.AttackType < EAttackType::SMASH)
		return;

	TWeakObjectPtr<AActor> GimicSubject = GetSubject();
	if (_hitInfo.Attacker.IsValid() == false ||
		GimicSubject.IsValid() == false)
		return;

	FVector AttackFwd = _hitInfo.Attacker->GetActorForwardVector();
	FVector SubjectFwd = GimicSubject->GetActorForwardVector();

	double Dot = FVector::DotProduct(AttackFwd, SubjectFwd);
	
	// 정면 45도 기준 : 180.0f - 45.0f
	if (Dot < 0 && FMath::RadiansToDegrees(FMath::Acos(Dot)) > 135.0f) 
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

void UStaggerGimic::Start(TWeakObjectPtr<AActor> _subject, const FGimicParam& _param)
{
	Super::Start(_subject, _param);
	StaggerValue = _param.OptionValue;
}

void UStaggerGimic::Interrupt(const FHitInfo& _hitInfo)
{
	Super::Interrupt(_hitInfo);

	TWeakObjectPtr<AActor> GimicSubject = GetSubject();
	if (_hitInfo.Attacker.IsValid() == false ||
		GimicSubject.IsValid() == false)
		return;

	if (StaggerValue <= _hitInfo.StaggerDamage)
	{
		// 무력화 완료
		StaggerValue = 0;

		if (IGimicHandler* GimicHandler = Cast<IGimicHandler>(GimicSubject.Get()))
			GimicHandler->StopGimic(GetType()); // 저지
	}
	else
		StaggerValue -= _hitInfo.StaggerDamage; // 무력화 진행

}

TObjectPtr<UGimicAction> UGimicActionFactory::CreateGimic(UObject* _worldContext, EGimicType _type)
{
	if (nullptr == _worldContext)
		return nullptr;

	switch (_type)
	{
	case EGimicType::COUNTER:
		return NewObject<UCounterGimic>(_worldContext);
	case EGimicType::STAGGER:
		return NewObject<UStaggerGimic>(_worldContext);
	default:
		return nullptr;
	}
}