// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PlayerCameraMoveAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UPlayerCameraMoveAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCurveVector> AnimCurve;

	UPROPERTY(EditAnywhere)
	float Duration{1.0f};

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);

};
