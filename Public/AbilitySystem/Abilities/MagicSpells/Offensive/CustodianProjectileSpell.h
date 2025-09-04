// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CustodianDamageGameplayAbility.h"
#include "CustodianProjectileSpell.generated.h"

class ACustodianProjectile;
struct FGameplayTag;

// Use to create projectile spell abilities
UCLASS()
class CUSTODIAN_API UCustodianProjectileSpell : public UCustodianDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Spawn a single projectile
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	// Spawn multiple projectiles at once
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Projectile", meta = (ReturnDisplayName = "SpawnedProjectiles"))
	TArray<ACustodianProjectile*> SpawnBarrageProjectiles(const FGameplayTag& SocketTag, AActor* HomingTarget);

	// Launched spawned array of homing projectiles over the given period of time
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Projectile")
	void LaunchProjectilesOverTime(TArray<ACustodianProjectile*> InSpawnedProjectiles, AActor* InHomingTarget, float InTime);

	// Handle launching a single projectile functionality
	void LaunchSingleProjectile(TArray<ACustodianProjectile*> InSpawnedProjectiles, AActor* InHomingTarget, float InTime, int32 CurrentIndex);

	// This function calculates the direction in which the projectile must fly. It can only be used for the player character abilities! 
	UFUNCTION(BlueprintCallable, Category = "Offensive Spells|Projectile|Player Character", meta = (ReturnDisplayName = "OutDirection"))
	FVector CalculateProjectileDirection(ACustodianProjectile* InMissile);

	// Check if the ability can be activated
	bool CanActivateAbility();


	/*
	* Projectile Properties
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile Properties")
	TSubclassOf<ACustodianProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	float ProjectileSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	float HomingAccelerationMax = 3200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	float ProjectileInitialSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	float ProjectileMaxSpeed = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	bool bLaunchHomingProjectiles = false;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	float BarrageProjectileLifeSpan = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	FName AttachmentSocketName;

	/*
	* Projectile Properties end
	*/
};
