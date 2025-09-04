// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MagicSpells/Passive/CustodianPassiveSpellAbility.h"
#include "GameplayTagContainer.h"
#include "StoneSkinAbility.generated.h"

// This ability class grants the player additional Armor Class and protects from debuffs but also block Sprinting Ability
UCLASS()
class CUSTODIAN_API UStoneSkinAbility : public UCustodianPassiveSpellAbility
{
	GENERATED_BODY()
	
public:

	// Spell Description
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	// Spell Description end

protected:

	// Call this function on "EventActivateAbility" to increase character's Armor Class and block any Debuffs
	UFUNCTION(BlueprintCallable, Category = "Passive Abilities|Stone Skin Ability")
	void EnableStoneSkin();

	// Call this function on "EventDeactivateAbility" to set character's Armor Class back to normal and stop protecting from Debuffs
	UFUNCTION(BlueprintCallable, Category = "Passive Abilities|Stone Skin Ability")
	void DisableStoneSkin();

	UPROPERTY(EditDefaultsOnly, Category = "Stone Skin Properties")
	float GrantedArmorClass = 25.f;
};
