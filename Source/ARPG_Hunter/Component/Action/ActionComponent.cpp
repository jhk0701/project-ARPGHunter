
#include "Component/Action/ActionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Define/Enum.h"
#include "Interface/Effectable.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "Data/Action.h"
#include "Data/EffectData.h"
#include "SubObject/SubObject.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActionComponent::SpawnHitVFX(UNiagaraSystem* _vfx, const FVector& _location, float _roll, float _size)
{
	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		_vfx,
		_location, FRotator::ZeroRotator, FVector::OneVector,
		true, true,
		ENCPoolMethod::AutoRelease
	);
	NiagaraComp->SetVariableFloat(FName(TEXT("User.HitRoll")), _roll);
	NiagaraComp->SetVariableFloat(FName(TEXT("User.HitSize")), _size);
}

void UActionComponent::ActivateActionEffect(TObjectPtr<AActor> _target, const TArray<TObjectPtr<UEffectData>>& _effectArray)
{
	IEffectable* Effectable = Cast<IEffectable>(_target);
	if (Effectable == nullptr)
		return;

	for (const TObjectPtr<class UEffectData>& effectData : _effectArray)
	{
		FApplyEffectParam Param;
		Param.Subject = GetOwner();
		Param.EffectData = effectData;

		Effectable->ApplyEffect(Param);
	}
}

bool UActionComponent::ProcessAttack(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction, TWeakObjectPtr<AActor> _target)
{
	TWeakObjectPtr<UAction> ActionData = GetCurrentAction();
	EAttackDetailType DetailType = ActionData->ArrOption[_opt].Detail;

	if (DetailType > EAttackDetailType::MELEE_END)
	{
		// 원거리 방식 처리
		FSubObjectDeployParam DeployParam;
		DeployParam.DetailType = DetailType;
		DeployParam.SubObjectClass = ActionData->SubObjectClass;
		DeployParam.SubObjectConfig = ActionData->SubObjectConfig;

		DeploySubObject(DeployParam, _traceChannel, MoveTemp(_onHitAction), _target); // 기존에 받았던 람다는 Move로 이동 처리

		return true;
	}

	// 근거리 방식 처리
	TArray<FHitResult> HitResults;
	FTraceParam TraceParam;
	TraceParam.DetailType = DetailType;
	TraceParam.Size = ActionData->ArrOption[_opt].Size;
	TraceParam.Range = ActionData->ArrOption[_opt].Range;

	bool bIsHit = Trace(TraceParam, _traceChannel, HitResults);
	if (false == bIsHit)
		return bIsHit;

	if (_onHitAction)
		_onHitAction(HitResults); // 피격했다면 콜백 호출

	PostProcessAttack(_opt, HitResults);
	return bIsHit;
}

bool UActionComponent::Trace(const FTraceParam& _param, ECollisionChannel _traceChannel, TArray<FHitResult>& _outResults)
{
	bool bIsHit = false;

	FVector Fwd = GetOwner()->GetActorForwardVector();
	FVector Start = GetOwner()->GetActorLocation() + Fwd * 10.0f;
	FVector End = Start + Fwd * _param.Range;

	EDrawDebugTrace::Type DrawDebug = bShowTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	switch (_param.DetailType)
	{
	case EAttackDetailType::MELEE_FRONT:
		bIsHit = UKismetSystemLibrary::BoxTraceMulti(
			GetWorld(),
			Start, End,
			_param.Size,
			Fwd.Rotation(),
			UEngineTypes::ConvertToTraceType(_traceChannel),
			false, { GetOwner() },
			DrawDebug,
			_outResults,
			true
		);
		break;
	case EAttackDetailType::MELEE_AROUND:
		bIsHit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			Start, Start,
			_param.Range,
			UEngineTypes::ConvertToTraceType(_traceChannel),
			false, { GetOwner() },
			DrawDebug,
			_outResults,
			true
		);
		break;
	}

	return bIsHit;
}

void UActionComponent::DeploySubObject(const FSubObjectDeployParam& _param, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction, TWeakObjectPtr<AActor> _target)
{
	// 서브 오브젝트에게 공격 동작 위임
	UClass* SubObjectClass = _param.SubObjectClass;
	if (nullptr == SubObjectClass)
		return;

	// 투사체 발사
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	ASubObject* SubObj = Cast<ASubObject>(ObjectPool->Get(SubObjectClass));

	SubObj->Init(_param.SubObjectConfig, MoveTemp(_onHitAction));

	FVector FireStart;
	if (FirePointComp.IsValid())
		FireStart = FirePointComp->GetSocketLocation(FirePointSocketName);
	else
		FireStart = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f;

	FVector FireDir;
	switch (_param.DetailType)
	{
	case EAttackDetailType::RANGED_DIRECTIONAL:
		if (_target.IsValid())
		{
			FireDir = _target->GetActorLocation() - FireStart;
			FireDir.Normalize();

			if (FVector::DotProduct(FireDir, GetOwner()->GetActorForwardVector()) < 0.75f)
				FireDir = GetOwner()->GetActorForwardVector();
		}
		else
			FireDir = GetOwner()->GetActorForwardVector();
		break;
	}

	SubObj->SetActorLocation(FireStart);
	SubObj->Fire(GetOwner(), FireDir);
}
