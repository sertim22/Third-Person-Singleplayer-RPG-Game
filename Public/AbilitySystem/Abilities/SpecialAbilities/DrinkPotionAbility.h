// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "DrinkPotionAbility.generated.h"

UCLASS()
class CUSTODIAN_API UDrinkPotionAbility : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
protected:
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// Check if the ability can be activated
	bool CanActivateAbility();

	UFUNCTION(BlueprintCallable, Category = "Special Abilities|Drink Potion")
	void ApplyPotionEffect();
};
