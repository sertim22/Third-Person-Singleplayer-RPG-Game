// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/MagicSpells/Passive/CustodianPassiveSpellAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CustodianAbilitySystemComponent.h"

void UCustodianPassiveSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UCustodianAbilitySystemComponent* CustodianASC = Cast<UCustodianAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		CustodianASC->DeactivatePassiveAbility.AddUObject(this, &UCustodianPassiveSpellAbility::RecieveDeactivate);
	}
}

void UCustodianPassiveSpellAbility::RecieveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
