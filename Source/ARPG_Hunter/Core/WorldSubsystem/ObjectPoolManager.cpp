// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/WorldSubsystem/ObjectPoolManager.h"

UActorObjectPool::UActorObjectPool()
{
}

void UActorObjectPool::Init(TFunction<TObjectPtr<AActor>()> _createFunc, int _initialSize)
{
	CreateFunc = _createFunc;
	Container.Reserve(_initialSize);

	for (int i = 0; i < _initialSize; ++i)
	{
		TObjectPtr<AActor> inst = Create();
		DeactivateActor(inst);
	}
}

TObjectPtr<AActor> UActorObjectPool::Create()
{
	TObjectPtr<AActor> inst = CreateFunc();

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

	if (Pool.Dequeue(inst) == false)
		inst = Create();

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

void UObjectPoolManager::Register(UClass* _class, TFunction<TObjectPtr<AActor>()> _createFunc, int _initialSize)
{
	if (MapObjectPool.Find(_class) != nullptr)
		return;

	TObjectPtr<UActorObjectPool>& Pool = MapObjectPool.Add(_class, NewObject<UActorObjectPool>(this));
	Pool->Init(MoveTemp(_createFunc), _initialSize);
}

TObjectPtr<AActor> UObjectPoolManager::Get(UClass* _class)
{
	if (MapObjectPool.Find(_class) == nullptr)
		return nullptr;

	return MapObjectPool[_class]->Get();
}

void UObjectPoolManager::Release(UClass* _class, TObjectPtr<AActor> _inst)
{
	if (MapObjectPool.Find(_class) == nullptr)
		return;

	MapObjectPool[_class]->Release(_inst);
}
