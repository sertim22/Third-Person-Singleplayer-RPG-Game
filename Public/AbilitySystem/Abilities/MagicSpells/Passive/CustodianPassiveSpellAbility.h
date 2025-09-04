// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "CustodianPassiveSpellAbility.generated.h"

// This class is a parent to all Passive Spells abilities. If you wish to create a new one you must derive from it
UCLASS()
class CUSTODIAN_API UCustodianPassiveSpellAbility : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
public:

	// Bind "RecieveDeactivate" to "DeactivatePassiveAbility" delegate here
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// This function ends the equipped Passive Spell ability. Bind it to the "DeactivatePassiveAbility" delegate from the 
	// Custodian Ability System Component class in order to unequip and End the ability in "ServerEquipAbility" function. 
	void RecieveDeactivate(const FGameplayTag& AbilityTag);
};
