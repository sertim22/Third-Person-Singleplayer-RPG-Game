// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/MagicSpells/Offensive/CustodianProjectileSpell.h"
#include "FireBolt.generated.h"

// Class inherited from the UCustodianProjectileSpell class and used to create Cast FireBolt ability
UCLASS()
class CUSTODIAN_API UFireBolt : public UCustodianProjectileSpell
{
	GENERATED_BODY()
	
public:

	// Spell Description
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	// Spell Description end
	
	// Spawn a single firebolt
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|FireBolt")
	void SpawnFireBolt(const FGameplayTag& SocketTag);
};
