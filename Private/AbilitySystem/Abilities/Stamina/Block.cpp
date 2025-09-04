// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/Stamina/Block.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"


void UBlock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

bool UBlock::CanActivateAbility()
{
	if (UCustodianAbilitySystemLibrary::IsBlocking(Cast<ACharacter>(GetAvatarActorFromActorInfo()))) return true;
	else return false;
}
