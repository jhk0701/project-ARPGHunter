// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AttackNotify.h"
#include "Interface/AttackNotifyHandler.h"

void UAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IAttackNotifyHandler* Handler = Cast<IAttackNotifyHandler>(MeshComp->GetOwner());
	if (Handler)
		Handler->HandleAttackNotify(Option);
}
