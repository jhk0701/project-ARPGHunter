// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PipelineStateCache.h"
#include "ShaderPipelineCache.h"
#include "Core/GameMode/ARPGGameMode.h"
#include "TitleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API ATitleGameMode : public AARPGGameMode
{
	GENERATED_BODY()

public:
	ATitleGameMode();
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> TitleUIClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> TitleUIInst;

	FDelegateHandle HandleShaderCompileBegin;
	FDelegateHandle HandleShaderCompileComplete;
	FDelegateHandle HandleShaderCompileProgress;

public:
	UFUNCTION()
	void ClickNewGame();
	UFUNCTION()
	void ClickContinue();

	void OnShaderCompileBegin(uint32 Count, const FShaderPipelineCache::FShaderCachePrecompileContext& ShaderCachePrecompileContext);
	void OnShaderCompileComplete(uint32 Count, double Seconds, const FShaderPipelineCache::FShaderCachePrecompileContext& ShaderCachePrecompileContext);
	void OnShaderCompileProgress();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	void BindShaderCompileEvent();
	void UnbindShaderCompileEvent();
};
