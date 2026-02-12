// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/GimicNotify.h"
#include "Monster/BossMonster.h" 
// 현재는 직접 액터에 접근하고 있음
// 만약 보스 액터 외에 기믹을 수행해야하는 액터가 있다면 인터페이스로 변경할 것

void UGimicNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (TObjectPtr<ABossMonster> Boss = Cast<ABossMonster>(MeshComp->GetOwner()))
		Boss->StartGimic(Type, Value);
}
