// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GimicNotify.generated.h"

enum class EGimicType : uint8;

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UGimicNotify : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	EGimicType Type;

	UPROPERTY(EditAnywhere)
	uint16 Value{1};

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
