// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianDamageGameplayAbility.h"
#include "MeleeAttack.generated.h"

// This gameplay ability class goal is to handle melee combat ability with different attack directions, its montages and stagger mechanics
UCLASS()
class CUSTODIAN_API UMeleeAttack : public UCustodianDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Check if the Ability can be activated
	bool CanActivateAbility();
};
