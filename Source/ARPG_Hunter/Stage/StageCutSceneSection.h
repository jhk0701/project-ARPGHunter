// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Stage/StageSection.h"
#include "StageCutSceneSection.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API AStageCutSceneSection : public AStageSection
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Section|Setting")
	uint8 CutSceneIndex{0};

	UPROPERTY()
	class ALevelSequenceActor* CutScenePlayer;

public:
	virtual void BeginSection() override;
	
	UFUNCTION()
	void OnCutSceneEnd();

};
