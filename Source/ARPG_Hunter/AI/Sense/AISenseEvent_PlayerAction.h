// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISenseEvent.h"
#include "AISenseEvent_PlayerAction.generated.h"

USTRUCT()
struct FAIPlayerActionStimulusEvent 
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 ActionType = 0;
	UPROPERTY()
	float Range = 500.0f;
	UPROPERTY()
	FVector Location = FVector::ZeroVector;
	UPROPERTY()
	TObjectPtr<AActor> Instigator = nullptr;

	// 연동할 Sense 클래스 지정
	typedef class UAISense_PlayerAction FSenseClass;
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UAISenseEvent_PlayerAction : public UAISenseEvent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FAIPlayerActionStimulusEvent PlayerActionEvent;

	virtual FAISenseID GetSenseID() const override;
};
