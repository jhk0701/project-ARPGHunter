// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "OrientToTargetNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UOrientToTargetNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	TWeakObjectPtr<AActor> Target;

	float Progress{0.0f};
	UPROPERTY(EditAnywhere)
	float OrientSpeed{1.0f};

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
