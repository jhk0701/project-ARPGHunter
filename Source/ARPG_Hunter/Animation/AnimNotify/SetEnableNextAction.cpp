// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/SetEnableNextAction.h"
#include "Player/PlayerCharacter.h"

void USetEnableNextAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		PlayerChar->EnableNextAction(true);
}
