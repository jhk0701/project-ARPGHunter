// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/OrientToTargetNotifyState.h"

#include "Monster/MonsterBase.h"

void UOrientToTargetNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	TObjectPtr<AMonsterBase> Monster = Cast<AMonsterBase>(MeshComp->GetOwner());
	if (Monster == nullptr)
		return;

	Target = Monster->GetTarget();
	Progress = 0.0f;
}

void UOrientToTargetNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (Target.IsValid() == false)
		return;

	TObjectPtr<AActor> Subject = MeshComp->GetOwner();

	FVector Dir = Target->GetActorLocation() - Subject->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();

	Progress = FMath::Min(1.0f, Progress + FrameDeltaTime * OrientSpeed);
	FQuat NewQuat = FQuat::Slerp(Subject->GetActorForwardVector().ToOrientationQuat(), Dir.ToOrientationQuat(), Progress);
	Subject->SetActorRotation(NewQuat);
}

void UOrientToTargetNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	Target = nullptr;
}
