// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MagicSpells/Regenerative/SpawnAuraAbility.h"
#include "SpawnHealingAura.generated.h"

class UGameplayEffect;

// Class is responsible for creating ability that steadily increases Character's Health attribute
// when overlapping with the Spawned Healing Aura Effect Actor class
UCLASS()
class CUSTODIAN_API USpawnHealingAura : public USpawnAuraAbility
{
	GENERATED_BODY()
	
public:

	// Spell Description
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	// Spell Description end

private:

	// Calculates the Regeneration value at the current ability level
	float CalculateHealthRegeneration(int32 Level);
};
