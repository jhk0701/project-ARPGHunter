// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster/RegularMonster/RangedMonster.h"

#include "Define/Debug.h"

ARangedMonster::ARangedMonster()
{
}

void ARangedMonster::HandleAttackNotify(uint8 _opt)
{
	Super::HandleAttackNotify(_opt);

	// 투사체 발사
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Ranged Monster Attack"));
}