// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "CustodianAbilityTypes.h"
#include "CustodianDamageGameplayAbility.generated.h"

// This gameplay ability class is a parent to all abilites that are supposed to cause damage.
// If you want to add a new damage ability you must create it based on this class
UCLASS()
class CUSTODIAN_API UCustodianDamageGameplayAbility : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
public:

	// Apply damage gameplay effect to cause damage to the Target Actor 
	UFUNCTION(BlueprintCallable, Category = "Custodian Gameplay Ability|Damage")
	void CauseDamage(AActor* TargetActor);

	// Create damage effect parameters, such as: debuff chance, debuff duration, damage type and etc. 
	UFUNCTION(BlueprintPure, Category = "Custodian Gameplay Ability|Damage", meta = (ReturnDisplayName = "OutDamageEffectParams"))
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulseDirection = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;

	// Get relevant damage amount at the current character level
	UFUNCTION(BlueprintCallable, Category = "Custodian Gameplay Ability|Damage", meta = (ReturnDisplayName = "OutDamage"))
	float GetDamageAtLevel() const;

protected:

	/*
	* Damage Parameters
	*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Parameters")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	float KnockbackChance = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Parameters")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Parameters")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage Parameters")
	float RadialDamageOuterRadius = 0.f;

	/*
	* Damage Parameters end
	*/
};
