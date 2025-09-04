// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "SpawnAuraAbility.generated.h"

class ACustodianEffectActor;

// Class used to create Spawning Aura abilites. Its derived from the UCustodianGameplayAbility class
UCLASS()
class CUSTODIAN_API USpawnAuraAbility : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Call to spawn a specified aura class
	UFUNCTION(BlueprintCallable, Category = "Regenerative Spells|Spawn Aura", meta = (ReturnDisplayName = "SpawnedAura"))
	ACustodianEffectActor* SpawnAura();

	// Check if the ability can be activated
	bool CanActivateAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Aura Properties")
	TSubclassOf<UGameplayEffect> AbilityGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura Properties")
	TSubclassOf<ACustodianEffectActor> EffectActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aura Properties")
	float AuraZOffset = 1000.f;
};
