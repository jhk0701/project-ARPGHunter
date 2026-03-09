// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/PlayerCameraMoveAnimNotify.h"
#include "Player/PlayerCharacter.h"

void UPlayerCameraMoveAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (nullptr == AnimCurve)
		return;

	if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(MeshComp->GetOwner())) 
		PlayerChar->PlayCameraAnim(AnimCurve, Duration);
}
