// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/GimicNotify.h"
#include "Interface/GimicHandler.h"

void UGimicNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IGimicHandler* Handler = Cast<IGimicHandler>(MeshComp->GetOwner()))
		Handler->HandleGimicNotify(Type, Value);
}
