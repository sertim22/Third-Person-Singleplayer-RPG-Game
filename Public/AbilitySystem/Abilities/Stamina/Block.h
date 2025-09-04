// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "Block.generated.h"

// Use this gameplay ability class to create a Block functionality for bot AI and Player character classes.
// Its derived from UCustodianGameplayAbility class
UCLASS()
class CUSTODIAN_API UBlock : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Check if the ability can be activated
	bool CanActivateAbility();
};
