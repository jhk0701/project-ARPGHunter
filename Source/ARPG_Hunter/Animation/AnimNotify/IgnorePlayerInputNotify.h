// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "IgnorePlayerInputNotify.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UIgnorePlayerInputNotify : public UAnimNotify
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere)
	bool bIgnoreInput{false};

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
