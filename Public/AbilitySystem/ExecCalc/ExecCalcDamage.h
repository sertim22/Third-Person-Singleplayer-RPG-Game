// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalcDamage.generated.h"

// This class calculates damage by taking into consideration all of the damage effect parameters, attributes and characters'
// (SourceAvatar and TargetAvatar) Levels
UCLASS()
class CUSTODIAN_API UExecCalcDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UExecCalcDamage();

	// Calculate damage
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	// Function defines whic debuff type must be applied
	void DetermineDebuff(const FGameplayEffectSpec& Spec, const FGameplayEffectCustomExecutionParameters& ExecutionParams, FAggregatorEvaluateParameters& EvaluationParameters, const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToCaptureDefs) const;

private:

	// Returns additional damage value from the weapon equipped (it could be ranged or melee weapon)
	float FindAvatarWeaponPower(AActor* InSourceAvatar) const;

	// Returns ArmorClass variable from the CustodianCharacterBase. Use this to increase "TargetBlockChance" variable in Execute_Implementation
	float FindTargetArmorClass(AActor* InTargetAvatar) const;
};
