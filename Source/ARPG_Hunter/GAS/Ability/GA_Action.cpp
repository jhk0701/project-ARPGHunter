// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GA_Action.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
// #include "Abilities/GameplayAbility_Montage.h"

#include "GAS/ARPGTags.h"


UGA_Action::UGA_Action()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// AbilityTags.AddTag(); // BP에서 추가 -> 액션마다 부여해야하는 Tag들이 다름
	// 콤보 그래프 구조
	// 태그 기반 연결
	ActivationOwnedTags.AddTag(ARPGGameplayTags::Character_State_InAction);
}

void UGA_Action::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

	if (nullptr == Montage)
	{
		EndAbility(Handle, OwnerInfo, ActivationInfo, true, true);
		return;
	}

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, Montage, PlayRate, SectionName);
	if (nullptr == Task)
	{
		EndAbility(Handle, OwnerInfo, ActivationInfo, true, true);
		return;
	}

	Task->OnCompleted.AddDynamic(this, &UGA_Action::OnComplete);
	Task->OnCancelled.AddDynamic(this, &UGA_Action::OnCanceled);
	Task->OnInterrupted.AddDynamic(this, &UGA_Action::OnCanceled);

	Task->ReadyForActivation();
}

void UGA_Action::OnComplete()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_Action::OnCanceled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}
