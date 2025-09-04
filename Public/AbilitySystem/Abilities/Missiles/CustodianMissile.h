// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianDamageGameplayAbility.h"
#include "CustodianMissile.generated.h"

class AArrowMissile;
class USoundBase;
class UNiagaraSystem;
class AArrowMissile;
struct FGameplayTag;

// Use this gameplay ability class to create abilities such as: shooting arrows,
// shooting ballistas and etc.
UCLASS()
class CUSTODIAN_API UCustodianMissile : public UCustodianDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Spawns a missile object at the location of the Socket Tag passed in
	UFUNCTION(BlueprintCallable, Category = "Missile")
	AArrowMissile* SpawnMissile(const FGameplayTag& SocketTag);

	// Calculates the direction and the velocity of the spawned missile object, by passing in Draw Strength. Can be used for player only!
	UFUNCTION(BlueprintCallable, Category = "Missile")
	void CalculateMissileDirectionAndSpeed(AArrowMissile* InMissile, float DrawStrength);

	// Calculates the direction and the velocity of the spawned missile object, by passing in Draw Strength. Can be used for AI only!
	UFUNCTION(BlueprintCallable, Category = "Missile")
	void CalculateMissileDirectionAndSpeedForAI(AArrowMissile* InMissile, float DrawStrength, const FVector& TargetLocation);

	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<AArrowMissile> ArrowMissileClass;

	// Calculates the direction of the spawned missile
	FVector CalculateMissileDirection(AArrowMissile* InMissile);

	// Check if the ability can be activated
	bool CanActivateAbility();
};
