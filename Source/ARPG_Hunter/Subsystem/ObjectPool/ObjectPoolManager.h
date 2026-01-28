// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolManager.generated.h"



UCLASS()
class ARPG_HUNTER_API UActorObjectPool : public UObject
{
	GENERATED_BODY()
public:
	UActorObjectPool();

private:
	TSubclassOf<AActor> ActorClass;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> Container;
	TQueue<TObjectPtr<AActor>> Pool;

	TObjectPtr<AActor> Create();
	void ActivateActor(TObjectPtr<AActor> _actor);
	void DeactivateActor(TObjectPtr<AActor> _actor);

public:
	void Init(TSubclassOf<AActor> _actorClass, int _initialSize);
	TObjectPtr<AActor> Get();
	void Release(TObjectPtr<AActor> _instance);
};

/**
 * 
 */
UCLASS()
class ARPG_HUNTER_API UObjectPoolManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UObjectPoolManager();
private:
	UPROPERTY()
	TMap<UClass*, TObjectPtr<UActorObjectPool>> MapObjectPool;

public:
	void RegisterObject(TSubclassOf<AActor> _class, int _initialSize = 30);
	TObjectPtr<AActor> Get(TSubclassOf<AActor> _class);
	void Release(TObjectPtr<AActor> _inst);
};
