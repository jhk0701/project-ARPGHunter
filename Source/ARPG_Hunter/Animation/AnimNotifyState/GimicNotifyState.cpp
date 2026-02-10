// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/GimicNotifyState.h"
#include "Monster/BossMonster/BossMonster.h"

void UGimicNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	TObjectPtr<ABossMonster> GimicSubject = Cast<ABossMonster>(MeshComp->GetOwner());
	if (nullptr == GimicSubject)
		return;

	GimicSubject->StartGimic(Type);
}

void UGimicNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	TObjectPtr<ABossMonster> GimicSubject = Cast<ABossMonster>(MeshComp->GetOwner());
	if (nullptr == GimicSubject)
		return;

	GimicSubject->ProceedGimic();
}

void UGimicNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	TObjectPtr<ABossMonster> GimicSubject = Cast<ABossMonster>(MeshComp->GetOwner());
	if (nullptr == GimicSubject)
		return;

	GimicSubject->CompleteGimic();
}
