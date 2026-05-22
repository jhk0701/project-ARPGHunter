// Fill out your copyright notice in the Description page of Project Settings.

#include "GAS/Ability/GA_CharacterMove.h"
#include "Player/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"

#include "GAS/ARPGTags.h"


UGA_CharacterMove::UGA_CharacterMove()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityTags.AddTag(ARPGGameplayTags::Character_Ability_Move);
}

void UGA_CharacterMove::ActivateAbility(
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

	if (APlayerCharacter* Character = Cast<APlayerCharacter>(ActorInfo->AvatarActor))
	{
		const FVector2D& DIR = Character->GetInputDirection();
		
		FRotationMatrix RotMatrix(FRotator(0.0, Character->GetControlRotation().Yaw, 0.0));
		FVector Fwd = RotMatrix.GetUnitAxis(EAxis::Y);
		FVector Rht = RotMatrix.GetUnitAxis(EAxis::X);

		Character->AddMovementInput(Fwd, DIR.Y);
		Character->AddMovementInput(Rht, DIR.X);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
