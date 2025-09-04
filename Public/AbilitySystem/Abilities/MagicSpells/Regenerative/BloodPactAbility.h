// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "BloodPactAbility.generated.h"

class UNiagaraSystem;

// Ability class that is used to create "Blood Pact" Regenerative Spell. 
// It causes damage to the Actor that activating this ability but fills the Mana
UCLASS()
class CUSTODIAN_API UBloodPactAbility : public UCustodianGameplayAbility
{
	GENERATED_BODY()

public:

	// Spell Description
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	// Spell Description end

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// Check if the ability can be activated
	bool CanActivateAbility();

	// Applies BloodPactGameplayEffect
	UFUNCTION(BlueprintCallable, Category = "Regenerative Spells|Blood Pact")
	void ApplyBloodPact();

	UPROPERTY(EditDefaultsOnly, Category = "Blood Pact Properties")
	TSubclassOf<UGameplayEffect> BloodPactGameplayEffectClass;

private:

	// Calculates the Health Cost value at the current ability level
	float CalculateHealthDamage(int32 Level);
};
