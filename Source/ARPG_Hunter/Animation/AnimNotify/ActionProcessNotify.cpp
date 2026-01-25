// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/ActionProcessNotify.h"
#include "Player/PlayerCharacter.h"

void UActionProcessNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		PlayerChar->SetActionProcess(ProcessType);
}
