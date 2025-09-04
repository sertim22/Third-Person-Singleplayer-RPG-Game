// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MeleeAttack.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/PlayerInterface.h"


void UMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
}

bool UMeleeAttack::CanActivateAbility()
{
	return UCustodianAbilitySystemLibrary::IsInMeleeStance(Cast<ACharacter>(GetAvatarActorFromActorInfo()));
}
