// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianDamageGameplayAbility.h"
#include "SpellTrap.generated.h"

class ACustodianEffectActor;

// This class is a parent to all SpellTraps abilities. Derive from it if you wish to create a new One.
// The class handles Spawning and Calculating loaction of the Trap
UCLASS()
class CUSTODIAN_API USpellTrap : public UCustodianDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	// Check if the ability can be activated
	bool CanActivateAbility();

	// This function calculates the spawn location of the Spell Trap 
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Spell Trap", meta = (ReturnDisplayName = "OutLocation"))
	FVector CalculateTrapSpawnLocation();

	// Spawns an EffectActor assigned on the "EffectActorClass" variable
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Spell Trap", meta = (ReturnDisplayName = "SpawnedTrap"))
	ACustodianEffectActor* SpawnSpellTrap();

	UPROPERTY(Category = "Spell Trap Properties", EditDefaultsOnly)
	TSubclassOf<ACustodianEffectActor> EffectActorClass;

	// The MAX distance at which it is possible to spawn a Spell Trap
	UPROPERTY(Category = "Spell Trap Properties", EditDefaultsOnly)
	float MaxSpawnDistance = 2500.f;
};
