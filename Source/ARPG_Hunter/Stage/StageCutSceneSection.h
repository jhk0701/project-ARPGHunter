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

public:
	AStageCutSceneSection();

private:
	UPROPERTY(EditAnywhere, Category = "Section|Setting")
	uint8 CutSceneIndex{0};
	UPROPERTY()
	TSoftObjectPtr<class ULevelSequence> CutSceneAssetPath;
	UPROPERTY()
	TObjectPtr<class ULevelSequence> CutSceneAsset;
	UPROPERTY()
	class ALevelSequenceActor* CutScenePlayer;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUWCutScene> CutSceneUIClass;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UUWCutScene> CutSceneUI;

protected:
	virtual void BeginPlay() override;

public:
	virtual void BeginSection() override;
	
	void OnCutSceneAssetLoaded();
	UFUNCTION()
	void OnCutSceneEnd();
	void SkipCutScene();
};
