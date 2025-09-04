// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

// This interface class belongs to the Enemy Character Class only.
class CUSTODIAN_API IEnemyInterface
{
	GENERATED_BODY()

public:

	// Call this event to set combat target
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface|Combat")
	void SetCombatTarget(AActor* InCombatTarget);

	// Call this event to get current Combat target
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Enemy Interface|Combat", meta = (ReturnDisplayName = "CombatTarget"))
	AActor* GetCombatTarget() const;

	// Call this event to free reserved tokens when character dies 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Enemy Interface|Combat")
	void ReturnAttackTokensOnDeath();

	// Checks if the Combat Target is currently PreAttacking or Attacking
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Enemy Interface|Combat", meta = (ReturnDisplayName = "IsAttacking"))
	bool IsCombatTargetAttacking();
};
