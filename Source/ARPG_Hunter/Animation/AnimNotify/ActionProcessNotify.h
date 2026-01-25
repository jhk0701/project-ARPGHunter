// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Define/Enum.h"
#include "ActionProcessNotify.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UActionProcessNotify : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess="true"))
	EActionProcess ProcessType;

public:
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
