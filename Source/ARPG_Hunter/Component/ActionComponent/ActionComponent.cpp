
#include "Component/ActionComponent/ActionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"

#include "Define/Enum.h"
#include "Interface/Effectable.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "SubObject/SubObject.h"
#include "Data/Action.h"
#include "Data/EffectData.h"


UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActionComponent::Init(TObjectPtr<UAnimInstance> _ownerAnimInstance, TObjectPtr<USkeletalMeshComponent> _firePointComp)
{
	OwnerAnimInstance = _ownerAnimInstance;
	FirePointComp = _firePointComp;
	CurrentAction = nullptr;
}

void UActionComponent::ProcessAttack(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction)
{
	if (nullptr == CurrentAction || CurrentAction->ArrOption.Num() <= _opt)
		return;

	if (CurrentAction->ArrOption[_opt].Detail > EAttackDetailType::MELEE_END)
	{
		// 원거리 방식 처리
		Deploy(_opt, _traceChannel, MoveTemp(_onHitAction)); // 기존에 받았던 람다는 Move로 이동 처리
		return;
	}

	// 근거리 방식 처리
	TArray<FHitResult> HitResults;
	if (Trace(_opt, _traceChannel, HitResults) == false)
		return;

	// 공격 히트 시, 효과 발동
	if (_onHitAction)
		_onHitAction(HitResults);

	// 자기 버프 적용
	ActivateActionEffect(CurrentAction->EffectOnHit, GetOwner());

	// 적에게 디버프 적용
	for (const FHitResult& Result : HitResults)
	{
		ActivateActionEffect(CurrentAction->EffectOnEnemyHit, Result.GetActor());

		// 피격 효과 출력
		if (CurrentAction->VFXOnHit)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				CurrentAction->VFXOnHit,
				Result.ImpactPoint,
				Result.ImpactNormal.Rotation(),
				FVector::OneVector,
				true,
				true,
				ENCPoolMethod::AutoRelease
			);
		}
	}
}

void UActionComponent::ActivateActionEffect(const TArray<TObjectPtr<class UEffectData>>& _effectArray, TObjectPtr<AActor> _target)
{
	IEffectable* Effectable = Cast<IEffectable>(_target);
	if (Effectable == nullptr)
		return;

	for (const TObjectPtr<class UEffectData>& effectData : _effectArray)
		Effectable->ApplyEffect(effectData);
}

TObjectPtr<UAnimMontage> UActionComponent::GetCurrentMontage()
{
	if (CurrentAction)
		return CurrentAction->Montage;

	return nullptr;
}

bool UActionComponent::Trace(uint8 _opt, ECollisionChannel _traceChannel, TArray<FHitResult>& _outResults)
{
	const FActionOption& Option = CurrentAction->ArrOption[_opt];
	bool bIsHit = false;

	FVector Fwd = GetOwner()->GetActorForwardVector();
	FVector Start = GetOwner()->GetActorLocation() + Fwd * 50.0f;
	FVector End = Start + Fwd * Option.Range;

	switch (Option.Detail)
	{
	case EAttackDetailType::MELEE_FRONT:
		bIsHit = UKismetSystemLibrary::BoxTraceMulti(
			GetWorld(), 
			Start, End,
			Option.Size,
			Fwd.Rotation(),
			UEngineTypes::ConvertToTraceType(_traceChannel),
			false, { GetOwner() },
			EDrawDebugTrace::None,
			_outResults,
			true
		);
		break;
	case EAttackDetailType::MELEE_AROUND:
		bIsHit = UKismetSystemLibrary::SphereTraceMulti(
			GetWorld(),
			Start, Start,
			Option.Range,
			UEngineTypes::ConvertToTraceType(_traceChannel),
			false, { GetOwner() },
			EDrawDebugTrace::None,
			_outResults,
			true
		);
		break;
	}

	return bIsHit;
}

void UActionComponent::Deploy(uint8 _opt, ECollisionChannel _traceChannel, TFunction<void(TArray<FHitResult>&)> _onHitAction)
{
	// 서브 오브젝트에게 공격 동작 위임
	UClass* SubObjectClass = CurrentAction->SubObjectClass;
	if (nullptr == SubObjectClass)
		return;

	// 투사체 발사
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	TObjectPtr<ASubObject> SubObj = Cast<ASubObject>(ObjectPool->Get(SubObjectClass));
	SubObj->Init(CurrentAction->SubObjectConfig, MoveTemp(_onHitAction));
	
	FVector FireVector;
	switch (CurrentAction->ArrOption[_opt].Detail)
	{
	case EAttackDetailType::RANGED_DIRECTIONAL:
		FireVector = GetOwner()->GetActorForwardVector();
		break;
	}
	
	if (FirePointComp)
		SubObj->SetActorLocation(FirePointComp->GetSocketLocation(FirePointSocketName));
	else
		SubObj->SetActorLocation(GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f);

	SubObj->Fire(GetOwner(), FireVector);
}
