// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CustodianGameplayAbility.generated.h"

// This is a parent to all abilities in this project. If you want to make a new ability 
// you must derive it from this class. In case you want to create an ability that causes damage
// create it based on UcustodianDamageGameplayAbility class
UCLASS()
class CUSTODIAN_API UCustodianGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	// This parameter defines what input tag you are going to use to activate the ability
	UPROPERTY(EditDefaultsOnly, Category = "Input Properties")
	FGameplayTag StartupInputTag;

	// Get Ability Description at the current level
	virtual FString GetDescription(int32 Level);
	// Get next level Ability Description
	virtual FString GetNextLevelDescription(int32 Level);
	// Get Description of the locked ability
	static FString GetLockedDescription(int32 Level);

protected:

	// Get Mana cost of the ability
	float GetManaCost(float InLevel = 1.f) const;
	// Get Cooldown of the ability
	float GetCooldown(float InLevel = 1.f) const;
};
