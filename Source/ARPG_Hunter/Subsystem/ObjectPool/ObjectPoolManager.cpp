// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/ObjectPool/ObjectPoolManager.h"

UActorObjectPool::UActorObjectPool()
{
}

void UActorObjectPool::Init(TSubclassOf<AActor> _actorClass, int _initialSize)
{
	ActorClass = _actorClass;
	Container.Reserve(_initialSize);

	for (int i = 0; i < _initialSize; ++i)
	{
		TObjectPtr<AActor> inst = Create();
		DeactivateActor(inst);
	}
}

TObjectPtr<AActor> UActorObjectPool::Create()
{
	TObjectPtr<AActor> inst = GetWorld()->SpawnActor(ActorClass);

	Container.Add(inst);
	Pool.Enqueue(inst);

	return inst;
}

void UActorObjectPool::ActivateActor(TObjectPtr<AActor> _actor)
{
	_actor->SetActorHiddenInGame(false);
	_actor->SetActorEnableCollision(true);
}

void UActorObjectPool::DeactivateActor(TObjectPtr<AActor> _actor)
{
	_actor->SetActorHiddenInGame(true);
	_actor->SetActorEnableCollision(false);
}

TObjectPtr<AActor> UActorObjectPool::Get()
{
	TObjectPtr<AActor> inst;

	if (Pool.IsEmpty())
		inst = Create();
	else
		Pool.Dequeue(inst);

	ActivateActor(inst);

	return inst;
}

void UActorObjectPool::Release(TObjectPtr<AActor> _instance)
{
	DeactivateActor(_instance);
	Pool.Enqueue(_instance);
}


UObjectPoolManager::UObjectPoolManager()
{
}

void UObjectPoolManager::RegisterObject(TSubclassOf<AActor> _class, int _initialSize)
{
	if (MapObjectPool.Find(_class) != nullptr)
		return;

	TObjectPtr<UActorObjectPool> Pool = MapObjectPool.Add(_class.Get());
	Pool->Init(_class, _initialSize);
}

TObjectPtr<AActor> UObjectPoolManager::Get(TSubclassOf<AActor> _class)
{
	if (MapObjectPool.Find(_class.Get()) == nullptr)
		return nullptr;

	return MapObjectPool[_class.Get()]->Get();
}

void UObjectPoolManager::Release(TObjectPtr<AActor> _inst)
{
	if (MapObjectPool.Find(_inst.GetClass()) == nullptr)
		return;

	MapObjectPool[_inst.GetClass()]->Release(_inst);
}
