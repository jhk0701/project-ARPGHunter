// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Interface/AttackNotifyHandler.h"
#include "AttackNotify.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UAttackNotify : public UAnimNotify
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	EAttackDirection AttackDirection;

public :
	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
