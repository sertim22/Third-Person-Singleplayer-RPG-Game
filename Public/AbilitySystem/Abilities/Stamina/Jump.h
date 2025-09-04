// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "Jump.generated.h"

// Use this class to create Jump ability
UCLASS()
class CUSTODIAN_API UJump : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Launch character
	UFUNCTION(BlueprintCallable, Category = "Jump Ability")
	void Jump();

	// Call to end the jump
	UFUNCTION(BlueprintCallable, Category = "Jump Ability")
	void StopJump();

	// Check if the ability can be activated
	bool CanActivateAbility();
};
