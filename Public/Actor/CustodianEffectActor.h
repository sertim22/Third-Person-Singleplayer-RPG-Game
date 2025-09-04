// Copyright Sertim all rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "CustodianAbilityTypes.h"
#include "CustodianEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectActorSpawnSignature, AActor*, TargetActor);

// Enum containing effect aplication policies
UENUM(BlueprintType)
enum class EEffectApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

// Enum containing effect removal policies
UENUM(BlueprintType)
enum class EEffectRemovalPolicy
{
	RemoveOnEndOverlap,
	DoNotRemove
};

// Struct that contains Gameplay Effect, Application and Removal policies of the Effect Actor
USTRUCT(BlueprintType)
struct FEffectType
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> GameplayEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy ApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy RemovalPolicy = EEffectRemovalPolicy::DoNotRemove;
};

// This class should be used if you are planning on adding actors that must apply Gameplay Effects to the actors overlapped with it
UCLASS()
class CUSTODIAN_API ACustodianEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACustodianEffectActor();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Actor|CustodianEffectActor")
	FOnEffectActorSpawnSignature OnEffectActorSpawnDelegate;

	UPROPERTY(BlueprintReadWrite, meta = (ExposedOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

protected:
	
	virtual void BeginPlay() override;

	// Applies Gameplay Effect to the Target Actor
	UFUNCTION(BlueprintCallable, Category = "Actor|CustodianEffectActor")
	void ApplyEffectToTarget(AActor* TargetActor, const FEffectType& GameplayEffectClass);

	// Calls ApplyEffectToTarget when TargetActor overlaps with the EffectActor
	UFUNCTION(BlueprintCallable, Category = "Actor|CustodianEffectActor")
	void OnOverlap(AActor* TargetActor);

	// Removes current active Gameplay Effect from Target Actor or it calls ApplyEffectToTarget when TargetActor
	// no longer overlaps with the EffectActor
	UFUNCTION(BlueprintCallable, Category = "Actor|CustodianEffectActor")
	void OnEndOverlap(AActor* TargetActor);

	// Checks if there was an overlap and prevents owner of the Actor from hitting itself
	bool IsValidOverlap(AActor* OtherActor);

	/*
	* Effect Actor Properties
	*/

	// Set this variable based on whether you are goin to apply damage or some other effect
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Actor Properties")
	bool bIsDamageEffect = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Actor Properties")
	USoundBase* OnOverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Actor Properties")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Actor Properties")
	bool bApplyEffectsToEnemies = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect Actor Properties")
	TArray<FEffectType> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Actor Properties")
	float ActorLevel = 1.f;

	/*
	* Effect Actor Properties end
	*/


	// A TMap that converts GameplayEffects into AbilitySystemComponents. Use it to add or remove gameplay effects from the Overlapped Actor
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
};
