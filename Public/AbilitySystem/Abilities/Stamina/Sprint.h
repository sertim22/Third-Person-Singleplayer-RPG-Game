// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianGameplayAbility.h"
#include "Sprint.generated.h"

// Use this class to create Sprint ability
UCLASS()
class CUSTODIAN_API USprint : public UCustodianGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Sets character's speed to the highest value to start sprinting
	UFUNCTION(BlueprintCallable, Category = "Sprint Ability")
	void StartSprint();

	// Set character's speed back to normal to stop sprinting
	UFUNCTION(BlueprintCallable, Category = "Sprint Ability")
	void StopSprint();

	// Check if the ability can be activated
	bool CanActivateAbility();


	/*
	* Sprint Properties
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Sprint Properties")
	float SprintSpeed = 780.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sprint Properties")
	float JoggingSpeed = 500.f;

	/*
	* Sprint Properties end
	*/
};
