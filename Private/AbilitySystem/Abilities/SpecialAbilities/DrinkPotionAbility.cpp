// Copyright Sertim all rights reserved


#include "AbilitySystem/Abilities/SpecialAbilities/DrinkPotionAbility.h"
#include "AbilitySystem/CustodianAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "Inventory/CustodianInventoryComponent.h"
#include "Interaction/InventoryInterface.h"
#include "AssistantFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/PlayerInterface.h"


void UDrinkPotionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (CanActivateAbility())
	{
		Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	}
	else return;
}

bool UDrinkPotionAbility::CanActivateAbility()
{
	// For Player
	if (GetAvatarActorFromActorInfo()->Implements<UPlayerInterface>())
	{
		if (!ICombatInterface::Execute_GetIsAttacking(GetAvatarActorFromActorInfo())
			&& !UCustodianAbilitySystemLibrary::IsAiming(Cast<ACharacter>(GetAvatarActorFromActorInfo()))
			&& UAssistantFunctionLibrary::GetIsWieldingConsumable(Cast<ACharacter>(GetAvatarActorFromActorInfo())) == true)
			return true;
		else return false;
	}
	// For AI
	else
	{
		if (!ICombatInterface::Execute_GetIsAttacking(GetAvatarActorFromActorInfo())) return true;
		else return false;
	}
}

void UDrinkPotionAbility::ApplyPotionEffect()
{
	UCustodianInventoryComponent* InvComponent = IInventoryInterface::
		Execute_GetCustodianInventoryComponent(GetAvatarActorFromActorInfo());

	if (InvComponent)
	{
		TSubclassOf<UGameplayEffect> EffectClass = UAssistantFunctionLibrary::
			GetConsumableEffectClassByTag(InvComponent->ConsumablesTag, InvComponent);

		ApplyGameplayEffectSpecToOwner(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo,
			MakeOutgoingGameplayEffectSpec(EffectClass));

		IPlayerInterface::Execute_ConsumableWasUsed(GetAvatarActorFromActorInfo());
	}
}
