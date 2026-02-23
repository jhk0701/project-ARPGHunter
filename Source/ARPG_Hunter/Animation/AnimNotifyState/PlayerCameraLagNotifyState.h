// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PlayerCameraLagNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UPlayerCameraLagNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Setting")
	float CameraLagSpeed = 1.0f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
