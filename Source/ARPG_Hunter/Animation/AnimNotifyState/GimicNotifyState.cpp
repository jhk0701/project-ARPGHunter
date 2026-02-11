// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/GimicNotifyState.h"
#include "Interface/GimicHandler.h"  

void UGimicNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	IGimicHandler* GimicSubject = Cast<IGimicHandler>(MeshComp->GetOwner());
	if (nullptr == GimicSubject)
		return;

	GimicSubject->StartGimic(Type);
}

void UGimicNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	IGimicHandler* GimicSubject = Cast<IGimicHandler>(MeshComp->GetOwner());
	if (nullptr == GimicSubject)
		return;

	GimicSubject->ProceedGimic(FrameDeltaTime);
}