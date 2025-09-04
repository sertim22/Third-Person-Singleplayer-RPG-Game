// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MagicSpells/Passive/CustodianPassiveSpellAbility.h"
#include "GameplayTagContainer.h"
#include "ColossusMightAbility.generated.h"

class UGameplayEffect;

// This ability class increases the size of the character and increases Strength attribute by the "GrantedStrength" value
UCLASS()
class CUSTODIAN_API UColossusMightAbility : public UCustodianPassiveSpellAbility
{
	GENERATED_BODY()
	
public:

	// Spell Description
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	// Spell Description end

protected:

	// Call this function on "EventActivateAbility" to increase character's Armor Class and block any Debuffs
	UFUNCTION(BlueprintCallable, Category = "Passive Abilities|Might of the Colossus Ability")
	void EnableCollossusMight();

	// Call this function on "EventDeactivateAbility" to set character's Armor Class back to normal and stop protecting from Debuffs
	UFUNCTION(BlueprintCallable, Category = "Passive Abilities|Might of the Colossus Ability")
	void DisableCollossusMight();

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	TSubclassOf<UGameplayEffect> EnableColossusMightEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	TSubclassOf<UGameplayEffect> DisableColossusMightEffectClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	float IncreasedCapsuleRadius = 100.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	float IncreasedCapsuleHalfHeight = 100.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	float ChangedMeshZOffset = -120.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	float IncreasedMeshScale = 2.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	float ChangedSpringArmTargetLength = 375.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Might of the Colossus Properties")
	float ChangedSpringArmZOffset = 45.f;

	float InitalCapsuleRadius = 0.f;
	float InitalCapsuleHalfHeight = 0.f;
	float InitialMeshZOffset = 0.f;
	FVector IntialMeshScale;
	float InitialSpringArmTargetLength = 0.f;
	float InitialSpringArmZOffset = 0.f;

private:

	// Calculates the Amount of Strength given by the "ColossusMightEffectClass"
	float CalculateStrengthUpgrade(int32 Level);
};
