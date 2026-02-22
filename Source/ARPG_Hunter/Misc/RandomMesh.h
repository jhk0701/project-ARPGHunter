// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomMesh.generated.h"

UCLASS()
class ARPG_HUNTER_API ARandomMesh : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxComp;

	UPROPERTY(EditAnywhere, Category = "Utility")
	TArray<TObjectPtr<UStaticMesh>> MeshList;
	UPROPERTY(EditAnywhere, Category = "Utility")
	bool bUpdateLocation{ false };
	UPROPERTY(EditAnywhere, Category = "Utility|RandomTransform")
	FVector MinLocationOffset;
	UPROPERTY(EditAnywhere, Category = "Utility|RandomTransform")
	FVector MaxLocationOffset;
	UPROPERTY(EditAnywhere, Category = "Utility|RandomTransform")
	FRotator MinRotation;
	UPROPERTY(EditAnywhere, Category = "Utility|RandomTransform")
	FRotator MaxRotation;
	UPROPERTY(EditAnywhere, Category = "Utility|RandomTransform")
	FVector MinScale{ 1.0f,1.0f,1.0f };
	UPROPERTY(EditAnywhere, Category = "Utility|RandomTransform")
	FVector MaxScale{ 1.0f,1.0f,1.0f };

public:	
	ARandomMesh();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Utility")
	void SetRandomMesh();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Utility")
	void SetRandomTransform();
};
