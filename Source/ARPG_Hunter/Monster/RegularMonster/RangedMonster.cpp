// Fill out your copyright notice in the Description page of Project Settings.

#include "Monster/RegularMonster/RangedMonster.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Controller/MonsterAIController.h"
#include "Core/WorldSubsystem/ObjectPoolManager.h"
#include "SubObject/SubObject.h"
#include "Data/MonsterData.h"
#include "Data/Action.h"

#include "Define/Debug.h"

ARangedMonster::ARangedMonster()
{
}

void ARangedMonster::HandleAttackNotify(uint8 _opt)
{
	Super::HandleAttackNotify(_opt);

	// 공격 목표 찾기
	AMonsterAIController* AICon = Cast<AMonsterAIController>(GetController());
	if (nullptr == AICon)
		return;

	UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
	UObject* Target = BBComp->GetValueAsObject(FName(TEXT("Target")));
	if (nullptr == Target)
		return;

	TObjectPtr<AActor> TargetActor = Cast<AActor>(Target);

	const FAttackDetail& Detail = GetCurAction()->AttackDetails[_opt];
	UClass* ProjectileClass = Detail.SubObjectClass;
	if (nullptr == ProjectileClass)
		return;

	// 투사체 발사
	UObjectPoolManager* ObjectPool = GetWorld()->GetSubsystem<UObjectPoolManager>();
	TObjectPtr<ASubObject> Projectile = Cast<ASubObject>(ObjectPool->Get(ProjectileClass));
	Projectile->Init(); // TODO : 투사체 데이터 삽입
	Projectile->SetActorLocation(GetWeaponComp()->GetSocketLocation(FName(TEXT("socket_firePoint"))));
	Projectile->Fire(this, TargetActor);
}