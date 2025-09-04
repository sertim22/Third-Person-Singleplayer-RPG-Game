// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustodianAbilityTypes.h"
#include "ArrowMissile.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;
class UAudioComponent;
class UNiagaraComponent;
class UParticleSystem;

// This class is parent to all missile type actors in game. If you are planning on making, for example an arrow, bolt or catapulte projectile
// you must derive from this class
UCLASS()
class CUSTODIAN_API AArrowMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AArrowMissile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UProjectileMovementComponent> MissileMovement;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(BlueprintReadWrite, meta = (ExposedOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	// Calculates the speed of the missile based on Draw Strength and direction
	UFUNCTION(BlueprintCallable, Category = "Actor|Missile")
	void CalculateMissileSpeed(const FVector& Direction,  UProjectileMovementComponent* InProjectileMovement, FVector& InVelocity, float Strength);

	// Call to turn on the sound and niagara or particle effect
	UFUNCTION(BlueprintCallable, Category = "Actor|Missile")
	void EnableArrowEffectsAndSounds();

protected:
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	// Call to stop and destroy sounds and effects when hit event fires
	UFUNCTION(BlueprintCallable, Category = "Actor|Missile")
	virtual void OnHit();

	// Calls OnHit, applies damage and reports damage event for AI
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromResult, const FHitResult& SweepResult);

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	// Checks if there was a hit and prevents owner of the Missile from hitting itself
	bool IsValidOverlap(AActor* OtherActor);

	bool bHit = false;
	bool bArrowHit = false;

private:

	/*
	* Missile Properties
	*/

	UPROPERTY(EditDefaultsOnly, Category = "Missile Properties")
	float LifeSpan = 30.f;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	TObjectPtr<UParticleSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	TObjectPtr<UNiagaraSystem> TrailEffect;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	TObjectPtr<USoundBase> FleshHitSound;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	TObjectPtr<USoundBase> WooshSound;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	float MinSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	float MaxSpeed = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	float MinGravity = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Missile Properties")
	float MaxGravity = 0.7f;

	/*
	* Missile Properties end
	*/

	// A reference to the spawned missile TrailEffect
	UNiagaraComponent* NiagaraEffect;
};
