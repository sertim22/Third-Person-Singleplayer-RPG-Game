// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianDamageGameplayAbility.h"
#include "MeteorBarrage.generated.h"

class ACustodianProjectile;
class UParticleSystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMeteorsEndedSignature, bool, MeteorsEnded);

// Class inherited from the UCustodianDamageGameplayAbility. Its used for creating Meteor Barage abilities
UCLASS()
class CUSTODIAN_API UMeteorBarrage : public UCustodianDamageGameplayAbility
{
	GENERATED_BODY()
	
public:

	// Delegate called when the ability has ended or was interrupted to call AttackEnd function.
	// In other words it notifes AI when the ability was ended
	UPROPERTY(BlueprintAssignable, Category = "Offensive Spells|Meteor Barrage")
	FOnMeteorsEndedSignature OnMeteorsEndedDelegate;

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Spawn meteors over time. This function can only be used by the player character!
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Meteor Barrage")
	void PlayerSpawnMeteors();

	// Spawn meteors over time. This function can only be used by the AI characters!
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Meteor Barrage")
	void AISpawnMeteors(AActor* InCombatTarget);

	// The function handles spawning meteors over time
	void SpawnMeteorsOverTime(TSubclassOf<ACustodianProjectile> InMeteorClass, int32 CurrentIndex, AActor* InCombatTarget);

	// Check if the ability can be activated
	bool CanActivateAbility();


	/*
	* Meteor Properties
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	TSubclassOf<ACustodianProjectile> MeteorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Meteor Properties")
	float MagicCircleZOffset = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Meteor Properties")
	float MeteorCloudZOffset = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	TObjectPtr<UParticleSystem> MeteorCloud;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	int32 MaxNumMeteors = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	float MeteorLifeSpan = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	float MinSpawnMeteorTime = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	float MaxSpawnMeteorTime = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	float MaxMeteorXOffset = 120.f;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	float MinMeteorXOffset = -120.f;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	float MaxMeteorYOffset = 120.f;

	UPROPERTY(EditDefaultsOnly, Category = "Meteor Properties")
	float MinMeteorYOffset = -120.f;

	/*
	* Meteor Properties end
	*/

	// Location calculated to spawn meteor cloud emitter
	FVector MeteorCloudSpawnLocation;
	// Sets the meteor spawn location
	void SetMeteorSpawnLocation(FVector InLocation);
	// Reference to the spawned cloud emitter
	UParticleSystemComponent* SpawnedCloudEmitter;
	bool bMeteorsEnded = false;
};
