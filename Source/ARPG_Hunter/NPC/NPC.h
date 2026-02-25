// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "NPC.generated.h"

class UUWPopUp;
class UUWNPCDialog;

UCLASS()
class ARPG_HUNTER_API ANPC : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPC();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UCapsuleComponent> ColliderComp;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere, Category = "NPC")
	TObjectPtr<class UNPCConfig> Config;

	UPROPERTY(EditAnywhere, Category = "NPC|UI")
	TSubclassOf<class UUWNPCDialog> DialogUIClass;
	UPROPERTY(VisibleAnywhere, Category = "NPC|UI")
	TObjectPtr<UUWNPCDialog> DialogUI;

	/// <summary>
	/// NPC 상호작용으로 파생된 UI는 NPC가 소유
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "NPC|UI")
	TMap<UClass*, TObjectPtr<UUWPopUp>> MapUIInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// IInteractable을(를) 통해 상속됨
	void Interact() override;
};