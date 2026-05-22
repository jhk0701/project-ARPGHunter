// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Ability/GA_CharacterSprint.h"
#include "GAS/ARPGTags.h"
#include "Character/ARPGCharacterBase.h"

UGA_CharacterSprint::UGA_CharacterSprint()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(ARPGGameplayTags::Character_Ability_Sprint);
}

void UGA_CharacterSprint::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (false == ActorInfo->AvatarActor.IsValid()) 
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (AARPGCharacterBase* Character = Cast<AARPGCharacterBase>(ActorInfo->AvatarActor))
		Character->SetMoveSpeed(true);
}

void UGA_CharacterSprint::EndAbility(
	const FGameplayAbilitySpecHandle Handle, 
	const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayAbilityActivationInfo ActivationInfo, 
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	if (AARPGCharacterBase* Character = Cast<AARPGCharacterBase>(ActorInfo->AvatarActor))
		Character->SetMoveSpeed(false);
}
