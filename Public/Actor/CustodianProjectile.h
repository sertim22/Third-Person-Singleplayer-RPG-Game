// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustodianAbilityTypes.h"
#include "CustodianProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UAudioComponent;
class UParticleSystem;
class UParticleSystemComponent;
class UNiagaraComponent;

// This class is parent to all magic related actors in the game that can be used as projectile.
// Use it to create Fire Bolts, Meteors and similar projectile actors
UCLASS()
class CUSTODIAN_API ACustodianProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACustodianProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	UPROPERTY(BlueprintReadWrite, meta = (ExposedOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	// Call to stop and destroy sounds and effects when hit event fires
	UFUNCTION(BlueprintCallable, Category = "Actor|Projectile")
	virtual void OnHit();

	// Calls OnHit, applies damage and reports damage event for AI
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult);

	// Checks if there was a hit and prevents owner of the Missile from hitting itself
	bool IsValidOverlap(AActor* OtherActor);

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	bool bHit = false;

private:

	/*
	* Projectile Properties
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	float LifeSpan = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	TObjectPtr<UParticleSystem> ProjectileParticleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	TObjectPtr<UNiagaraSystem> ProjectileNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Properties")
	TObjectPtr<USoundBase> LoopingSound;

	/*
	* Projectile Properties
	*/

	// Reference to the spawned particle effect
	UParticleSystemComponent* SpawnedParticleEffect;
	// Reference to the spawned niagara system
	UNiagaraComponent* SpawnedNiagaraSystem;
};
