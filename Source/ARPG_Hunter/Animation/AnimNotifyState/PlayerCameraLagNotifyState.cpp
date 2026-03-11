// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifyState/PlayerCameraLagNotifyState.h"
#include "Player/PlayerCharacter.h"

void UPlayerCameraLagNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (TObjectPtr<APlayerCharacter> PlayerChar = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		PlayerChar->SetCameraLag(true, CameraLagSpeed);
}

void UPlayerCameraLagNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (TObjectPtr<APlayerCharacter> PlayerChar = Cast<APlayerCharacter>(MeshComp->GetOwner()))
		PlayerChar->SetCameraLag(false);
}
